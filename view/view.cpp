#include "view/view.h"

#include <QColor>
#include <QDebug>
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
#include "view/mob_drawer.h"

View* View::GetInstance() {
  static View view;
  return &view;
}

View::View()
    : QOpenGLWidget(nullptr),
      camera_(QPointF(150, 150)),
      sound_manager_(new SoundManager()),
      drawer_(nullptr) {
  connect(Model::GetInstance(), &Model::DamageDealt, this, &View::DamageDealt);
  connect(Model::GetInstance(), &Model::BecameDead, this, &View::BecameDead);
  connect(Model::GetInstance(), &Model::MobSound, this, &View::MobSound);
}

void View::SetInventoryDrawer(InventoryDrawer* drawer) {
  inventory_drawer_.reset(drawer);
}

void View::initializeGL() {
  qDebug() << "GL init";
  assert(context());
  makeCurrent();
  auto* gl = GLFunctions::GetInstance();
  gl->initializeOpenGLFunctions();
  gl->glClearColor(1.0F, 1.0F, 1.0F, 1.0F);

  if (drawer_) {
    drawer_->Init();
  }
  qDebug() << "GL init success";
}

void View::paintGL() {
  auto* gl = GLFunctions::GetInstance();

  QPainter painter(this);
  painter.beginNativePainting();

  gl->glClear(GL_COLOR_BUFFER_BIT);

  camera_.SetPoint(Model::GetInstance()->GetPlayer()->GetPosition());
  drawer_->DrawMapWithCenter(&painter, camera_.GetPoint(), rect());

  inventory_drawer_->DrawInventory(&painter);

  // TODO(Wind-Eagle): temporary code; need to make PlayerDrawer
  auto player = Model::GetInstance()->GetPlayer();
  QImage player_image(":/resources/textures/player.png");
  QPointF point =
      (player->GetPosition() - camera_.GetPoint()) * constants::kBlockSz +
      rect().center();
  painter.drawImage(point, player_image);
  auto mobs = Model::GetInstance()->GetMobs();
  for (auto mob : mobs) {
    QPointF mob_point =
        (mob->GetPosition() - camera_.GetPoint()) * constants::kBlockSz +
        rect().center();
    MobDrawer::DrawMob(&painter, mob_point, mob);
  }
  painter.endNativePainting();
}

void View::DamageDealt(MovingObject::Type type) {
  static std::uniform_int_distribution<int> distrib(0, 1);
  switch (type) {
    case MovingObject::Type::kPlayer:
      sound_manager_->PlaySound(SoundManager::Sound::kPlayerDamage);
      break;
    case MovingObject::Type::kMob:
      if (distrib(utils::random) == 0) {
        sound_manager_->PlaySound(SoundManager::Sound::kMobDamage1);
      } else {
        sound_manager_->PlaySound(SoundManager::Sound::kMobDamage2);
      }
      break;
    default:
      break;
  }
}

void View::BecameDead(MovingObject::Type type) {
  Q_UNUSED(type);
  sound_manager_->PlaySound(SoundManager::Sound::kMobDeath);
}

void View::MobSound(MovingObject::Type type) {
  Q_UNUSED(type);
  static std::uniform_int_distribution<int> distrib(0, 1);
  if (distrib(utils::random) == 0) {
    sound_manager_->PlaySound(SoundManager::Sound::kMob1);
  } else {
    sound_manager_->PlaySound(SoundManager::Sound::kMob2);
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

QPointF View::GetTopLeftWindowCoord() const {
  return camera_.GetPoint() - QPointF(rect().center()) / constants::kBlockSz;
}
