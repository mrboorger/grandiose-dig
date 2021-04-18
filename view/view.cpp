#include "view/view.h"

#include <QColor>
#include <QPainter>
#include <cmath>
#include <ctime>
#include <random>

#include "controller/controller.h"
#include "model/constants.h"
#include "view/block_drawer.h"
#include "view/mob_drawer.h"

View* View::GetInstance() {
  static View view;
  return &view;
}

View::View() : QWidget(nullptr), camera_(QPointF(150, 150)), drawer_(nullptr) {
  sound_manager_ = new SoundManager();
  connect(Model::GetInstance(), &Model::DamageDealt, this, &View::DamageDealt);
  connect(Model::GetInstance(), &Model::BecameDead, this, &View::BecameDead);
  connect(Model::GetInstance(), &Model::MobSound, this, &View::MobSound);
}

void View::SetInventoryDrawer(InventoryDrawer* drawer) {
  inventory_drawer_.reset(drawer);
}

void View::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);
  QPainter painter(this);

  // TODO(degmuk): temporary code; replace with background drawer
  painter.setBrush(Qt::white);
  painter.drawRect(rect());

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
}

void View::DamageDealt(MovingObject::Type type) {
  qDebug() << "!";
  static std::mt19937 rnd(time(NULL));
  uint32_t sound = rnd();
  switch (type) {
    case MovingObject::Type::kPlayer:
      sound_manager_->PlaySound(SoundManager::Sound::kPlayerDamage);
      break;
    case MovingObject::Type::kMob:
      if (sound % 2 == 0) {
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
  sound_manager_->PlaySound(SoundManager::Sound::kMobDeath);
}

void View::MobSound(MovingObject::Type type) {
  static std::mt19937 rnd(time(NULL));
  uint32_t sound = rnd();
  if (sound % 2 == 0) {
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

QPoint View::GetBlockCoordUnderCursor() const {
  QPointF pos =
      GetTopLeftWindowCoord() + QPointF(GetCursorPos()) / constants::kBlockSz;
  return QPoint(std::floor(pos.x()), std::floor(pos.y()));
}

QPointF View::GetTopLeftWindowCoord() const {
  return camera_.GetPoint() - QPointF(rect().center()) / constants::kBlockSz;
}
