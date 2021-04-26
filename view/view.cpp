#include "view/view.h"

#include <QColor>
#include <QPainter>
#include <cmath>
#include <ctime>
#include <random>

#include "controller/controller.h"
#include "model/constants.h"
#include "utils.h"
#include "view/block_drawer.h"
#include "view/mob_drawer.h"

View* View::GetInstance() {
  static View view;
  return &view;
}

View::View()
    : QWidget(nullptr),
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

QString View::GetPlayerImage() {
  QString player_picture = ":/resources/textures/player";
  if (Model::GetInstance()->GetPlayer()->GetState() !=
      MovingObject::State::kWalk) {
    return player_picture + "0.png";
  }
  int state_time =
      std::floor(Model::GetInstance()->GetPlayer()->GetStateTime());
  int picture_number = (state_time / constants::kPlayerWalkAnimation) %
                       constants::kPlayerWalkPictures;
  return player_picture + QString::number(picture_number) + ".png";
}

QString View::GetPlayerAttackImage() {
  QString player_picture = ":/resources/textures/player_attack";
  int state_time = std::floor(
      constants::kPlayerAttackTime -
      Model::GetInstance()->GetPlayer()->GetAttackTick() - constants::kEps);
  int picture_number =
      (state_time /
       static_cast<int>((std::floor(constants::kPlayerAttackTime)) /
                        (constants::kPlayerAttackPictures - 1))) %
          (constants::kPlayerAttackAnimation - 1) +
      1;
  return player_picture + QString::number(picture_number) + ".png";
}

void View::DrawPlayer(QPainter* painter) {
  auto player = Model::GetInstance()->GetPlayer();
  QImage player_image(GetPlayerImage());
  if (Model::GetInstance()->GetPlayer()->GetDirection() ==
      utils::Direction::kLeft) {
    player_image = player_image.mirrored(true, false);
  }
  QPointF point =
      (player->GetPosition() - camera_.GetPoint()) * constants::kBlockSz +
      rect().center();

  if (Model::GetInstance()->GetPlayer()->IsAttackFinished() == false) {
    QImage player_attack_image(GetPlayerAttackImage());
    if (Model::GetInstance()->GetPlayer()->GetDirection() ==
        utils::Direction::kLeft) {
      player_attack_image = player_attack_image.mirrored(true, false);
    }
    player_image = player_image.copy(QRect(
        QPoint(0, player_attack_image.size().height()), player_image.size()));
    painter->drawImage(point + QPoint(0, player_attack_image.size().height()),
                       player_image);
    painter->drawImage(point, player_attack_image);
  } else {
    painter->drawImage(point, player_image);
  }
  // TODO(Wind-Eagle): make animation for block breaking: will be done when
  // blocks will not break immediately
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
  DrawPlayer(&painter);
  auto mobs = Model::GetInstance()->GetMobs();
  for (auto mob : mobs) {
    QPointF mob_point =
        (mob->GetPosition() - camera_.GetPoint()) * constants::kBlockSz +
        rect().center();
    MobDrawer::DrawMob(&painter, mob_point, mob);
  }
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
