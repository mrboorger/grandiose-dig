#include "view/view.h"

#include <QColor>
#include <QPainter>
#include <chrono>
#include <cmath>
#include <ctime>
#include <mutex>
#include <random>

#include "controller/controller.h"
#include "model/constants.h"
#include "utils.h"
#include "view/block_drawer.h"
#include "view/gl_func.h"
#include "view/moving_object_drawer.h"

View* View::instance_ = nullptr;

View* View::GetInstance() { return instance_; }

View::View()
    : QOpenGLWidget(nullptr),
      sound_manager_(new SoundManager()),
      drawer_(nullptr) {
  assert(!instance_);
  instance_ = this;
  connect(Model::GetInstance(), &Model::DamageDealt, this, &View::DamageDealt);
  connect(Model::GetInstance(), &Model::BecameDead, this, &View::BecameDead);
  connect(Model::GetInstance(), &Model::MobSound, this, &View::MobSound);
}

View::~View() {
  need_continue_ = false;
  update_light_thread_->join();
  instance_ = nullptr;
}

void View::SetInventoryDrawer(InventoryDrawer* drawer) {
  inventory_drawer_.reset(drawer);
}

void View::DrawPlayer(QPainter* painter) {
  auto player = Model::GetInstance()->GetPlayer();
  QPointF point = (player->GetPosition() - camera_pos_) * constants::kBlockSz +
                  rect().center();

  MovingObjectDrawer::DrawPlayer(painter, point);
  // TODO(Wind-Eagle): make animation for block breaking: will be done when
  // blocks will not break immediately
}

void View::initializeGL() {
  assert(context());
  makeCurrent();
  auto* gl = GLFunctions::GetInstance();
  gl->initializeOpenGLFunctions();
  gl->glEnable(GL_DEPTH_TEST);
  gl->glDepthFunc(GL_LESS);
  gl->glClearColor(1.0F, 1.0F, 1.0F, 1.0F);

  if (drawer_) {
    drawer_->Init();
  }
  if (auto player = Model::GetInstance()->GetPlayer()) {
    camera_pos_ = Model::GetInstance()->GetPlayer()->GetPosition();
  }
  update_light_thread_.reset(new std::thread([this] { UpdateLight(); }));
}

void View::paintGL() {
  auto* gl = GLFunctions::GetInstance();
  QPainter painter(this);
  painter.beginNativePainting();

  gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  camera_pos_ = Model::GetInstance()->GetPlayer()->GetPosition();

  auto to_update = light_map_->TakeUpdateList();
  for (auto pos : to_update) {
    drawer_->UpdateBlock(pos);
  }
  drawer_->DrawMapWithCenter(&painter, camera_pos_, rect());

  inventory_drawer_->DrawInventory(&painter);

  // TODO(Wind-Eagle): temporary code; need to make PlayerDrawer
  DrawPlayer(&painter);
  auto mobs = Model::GetInstance()->GetMobs();
  for (auto mob : mobs) {
    QPointF mob_point =
        (mob->GetPosition() - camera_pos_) * constants::kBlockSz +
        rect().center();
    MovingObjectDrawer::DrawMob(&painter, mob_point, mob);
  }
  painter.endNativePainting();
}

namespace {
double GetSoundVolume(MovingObject* object) {
  double dist =
      utils::GetDistance(static_cast<Mob*>(object)->GetPosition(),
                         Model::GetInstance()->GetPlayer()->GetPosition());
  double volume =
      std::pow(constants::kMobSoundC1, std::pow(dist, constants::kMobSoundC2));
  return volume * 100;
}

}  // namespace

void View::DamageDealt(MovingObject* object) {
  static std::uniform_int_distribution<int> distrib(0, 1);
  switch (object->GetType()) {
    case MovingObject::Type::kPlayer: {
      sound_manager_->PlaySound(
          SoundManager::SoundIndex(SoundManager::Sound::kPlayerDamage));
      break;
    }
    case MovingObject::Type::kMob: {
      int id = static_cast<Mob*>(object)->GetId();

      if (distrib(utils::random) == 0) {
        sound_manager_->PlaySound(
            SoundManager::SoundIndex(SoundManager::Sound::kMob, id,
                                     SoundManager::MobSound::kDamage1),
            GetSoundVolume(object));
      } else {
        sound_manager_->PlaySound(
            SoundManager::SoundIndex(SoundManager::Sound::kMob, id,
                                     SoundManager::MobSound::kDamage2),
            GetSoundVolume(object));
      }
      break;
    }
    default: {
      assert(false);
    }
  }
}

void View::BecameDead(MovingObject* object) {
  int id = static_cast<Mob*>(object)->GetId();
  sound_manager_->PlaySound(
      SoundManager::SoundIndex(SoundManager::Sound::kMob, id,
                               SoundManager::MobSound::kDeath),
      GetSoundVolume(object));
}

void View::MobSound(MovingObject* object) {
  int id = static_cast<Mob*>(object)->GetId();
  static std::uniform_int_distribution<int> distrib(0, 1);
  if (distrib(utils::random) == 0) {
    sound_manager_->PlaySound(
        SoundManager::SoundIndex(SoundManager::Sound::kMob, id,
                                 SoundManager::MobSound::kIdle1),
        GetSoundVolume(object));
  } else {
    sound_manager_->PlaySound(
        SoundManager::SoundIndex(SoundManager::Sound::kMob, id,
                                 SoundManager::MobSound::kIdle2),
        GetSoundVolume(object));
  }
}

void View::PlayMusic() {
  sound_manager_->PlaySound(
      SoundManager::SoundIndex(SoundManager::Sound::kMusic),
      constants::kMusicVolume);
}

void View::keyPressEvent(QKeyEvent* event) {
  Controller::GetInstance()->KeyPress(event->key());
}

void View::keyReleaseEvent(QKeyEvent* event) {
  Controller::GetInstance()->KeyRelease(event->key());
}

void View::mousePressEvent(QMouseEvent* event) {
  Controller::GetInstance()->ButtonPress(event->button());
}

void View::mouseReleaseEvent(QMouseEvent* event) {
  Controller::GetInstance()->ButtonRelease(event->button());
}

QPoint View::GetCursorPos() const {
  return QCursor::pos() - geometry().topLeft();
}

QPointF View::GetCoordUnderCursor() const {
  return GetTopLeftWindowCoord() +
         QPointF(GetCursorPos()) / constants::kBlockSz;
}

QPoint View::GetBlockCoordUnderCursor() const {
  QPointF pos = GetCoordUnderCursor();
  return QPoint(std::floor(pos.x()), std::floor(pos.y()));
}

void View::UpdateLight() {
  while (need_continue_) {
    light_map_->CalculateRegion(
        drawer_->GetDrawRegion(QPoint(camera_pos_.x(), camera_pos_.y())));
    std::this_thread::sleep_for(
        std::chrono::milliseconds(2 * constants::kTickDurationMsec));
  }
}

QPointF View::GetTopLeftWindowCoord() const {
  return camera_pos_ - QPointF(rect().center()) / constants::kBlockSz;
}
