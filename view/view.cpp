#include "view/view.h"

#include <QColor>
#include <QPainter>
#include <chrono>
#include <cmath>
#include <ctime>
#include <random>

#include "controller/controller.h"
#include "model/constants.h"
#include "utils.h"
#include "view/block_drawer.h"
#include "view/gl_func.h"
#include "view/moving_object_drawer.h"

View* View::instance_ = nullptr;

View* View::GetInstance() {
  return instance_;
}

View::View()
    : QOpenGLWidget(nullptr),
      camera_(QPointF(150, 150)),
      sound_manager_(new SoundManager()),
      drawer_(nullptr) {
  assert(!instance_);
  instance_ = this;
  connect(Model::GetInstance(), &Model::DamageDealt, this, &View::DamageDealt);
  connect(Model::GetInstance(), &Model::BecameDead, this, &View::BecameDead);
  connect(Model::GetInstance(), &Model::MobSound, this, &View::MobSound);
}

void View::SetInventoryDrawer(InventoryDrawer* drawer) {
  inventory_drawer_.reset(drawer);
}

void View::DrawPlayer(QPainter* painter) {
  auto player = Model::GetInstance()->GetPlayer();
  QPointF point =
      (player->GetPosition() - camera_.GetPoint()) * constants::kBlockSz +
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
  gl->glClearColor(1.0F, 1.0F, 1.0F, 1.0F);

  if (drawer_) {
    drawer_->Init();
  }
}

void View::paintGL() {
  auto* gl = GLFunctions::GetInstance();
  QPainter painter(this);
  painter.beginNativePainting();

  gl->glClear(GL_COLOR_BUFFER_BIT);

  camera_.SetPoint(Model::GetInstance()->GetPlayer()->GetPosition());
  QPointF camera_pos = camera_.GetPoint();

  UpdateLight(QPoint(camera_pos.x(), camera_pos.y()));
  drawer_->DrawMapWithCenter(&painter, camera_pos, rect());

  inventory_drawer_->DrawInventory(&painter);

  // TODO(Wind-Eagle): temporary code; need to make PlayerDrawer
  DrawPlayer(&painter);
  auto mobs = Model::GetInstance()->GetMobs();
  for (auto mob : mobs) {
    QPointF mob_point =
        (mob->GetPosition() - camera_.GetPoint()) * constants::kBlockSz +
        rect().center();
    MovingObjectDrawer::DrawMob(&painter, mob_point, mob);
  }
  painter.endNativePainting();
}

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
        sound_manager_->PlaySound(SoundManager::SoundIndex(
            SoundManager::Sound::kMob, id, SoundManager::MobSound::kDamage1));
      } else {
        sound_manager_->PlaySound(SoundManager::SoundIndex(
            SoundManager::Sound::kMob, id, SoundManager::MobSound::kDamage2));
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
  sound_manager_->PlaySound(SoundManager::SoundIndex(
      SoundManager::Sound::kMob, id, SoundManager::MobSound::kDeath));
}

void View::MobSound(MovingObject* object) {
  int id = static_cast<Mob*>(object)->GetId();
  static std::uniform_int_distribution<int> distrib(0, 1);
  if (distrib(utils::random) == 0) {
    sound_manager_->PlaySound(SoundManager::SoundIndex(
        SoundManager::Sound::kMob, id, SoundManager::MobSound::kIdle1));
  } else {
    sound_manager_->PlaySound(SoundManager::SoundIndex(
        SoundManager::Sound::kMob, id, SoundManager::MobSound::kIdle2));
  }
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

void View::UpdateLight(QPoint camera_pos) {
  light_map_->CalculateRegion(drawer_->GetDrawRegion(camera_pos));
  auto* to_update = light_map_->GetUpdateList();
  for (auto pos : *to_update) {
    drawer_->UpdateBlock(pos);
  }
  to_update->clear();
}

QPointF View::GetTopLeftWindowCoord() const {
  return camera_.GetPoint() - QPointF(rect().center()) / constants::kBlockSz;
}
