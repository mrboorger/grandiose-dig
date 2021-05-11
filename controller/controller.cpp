#include "controller/controller.h"

#include <algorithm>
#include <cmath>
#include <memory>
#include <utility>

#include "model/abstract_map_generator.h"
#include "model/chunk_map.h"
#include "model/constants.h"
#include "model/model.h"
#include "view/abstract_map_drawer.h"
#include "view/buffered_map_drawer.h"
#include "view/map_drawer.h"
#include "view/gl_map_drawer.h"
#include "view/view.h"

Controller* Controller::GetInstance() {
  static Controller controller;
  return &controller;
}

void Controller::SetGeneratedMap(AbstractMapGenerator* generator) {
  auto map = std::shared_ptr<AbstractMap>(generator->GenerateMap());
  Model::GetInstance()->SetMap(map);
  View::GetInstance()->SetLightMap(new LightMap(map));
  View::GetInstance()->SetDrawer(
      new GLMapDrawer(map, View::GetInstance()->GetLightMap()));
}

Controller::Controller() : tick_timer_() {
  tick_timer_.callOnTimeout([this]() { TickEvent(); });
  tick_timer_.start(constants::kTickDurationMsec);
}

void Controller::SetPlayer() {
  // TODO(Wind-Eagle): this is temporary code.
  Model::GetInstance()->SetPlayer(
      std::make_shared<Player>(QPointF(147.0, 110.0)));
  View::GetInstance()->SetInventoryDrawer(
      new InventoryDrawer(Model::GetInstance()->GetPlayer()->GetInventory()));
}

void Controller::SetMob() {
  // TODO(Wind-Eagle): this is temporary code.
  Model::GetInstance()->AddMob(
      std::make_shared<Mob>(QPointF(157.0, 107.0), QPointF(0.75, 1.75)));
}

void Controller::BreakBlock() {
  QPoint block_coords = View::GetInstance()->GetBlockCoordUnderCursor();
  if (Model::GetInstance()->GetMap()->HitBlock(block_coords, 1)) {
    View::GetInstance()->GetLightMap()->UpdateLight(block_coords);
  }
  View::GetInstance()->UpdateBlock(block_coords);
}

void Controller::StartAttack() {
  if (!Model::GetInstance()->GetPlayer()->CanStartAttack()) {
    return;
  }
  QPointF click_coord = View::GetInstance()->GetCoordUnderCursor();
  Model::GetInstance()->GetPlayer()->SetAttackDirection(
      (click_coord.x() < Model::GetInstance()->GetPlayer()->GetPosition().x())
          ? utils::Direction::kLeft
          : utils::Direction::kRight);
  Model::GetInstance()->GetPlayer()->SetAttackTick(
      constants::kPlayerAttackTime);
  Model::GetInstance()->GetPlayer()->SetAttackCooldownInterval(
      constants::kPlayerAttackCooldown);
}

bool Controller::IsVisible(QPointF player_center, QPointF mob_point) const {
  double x1 = player_center.x();
  double y1 = player_center.y();
  double x2 = mob_point.x();
  double y2 = mob_point.y();
  if (y1 < y2) {
    std::swap(x1, x2);
    std::swap(y1, y2);
  }
  QPointF old_center(x1, y1);
  QPointF new_center(x2, y2);
  int end_y = std::floor(new_center.y());
  int begin_y = std::floor(old_center.y());
  int dist = std::max(std::abs(end_y - begin_y), 1);
  for (int y = begin_y; y >= end_y; y--) {
    double center_left_x =
        utils::DivideSegment(old_center, new_center,
                             static_cast<double>(std::abs(end_y - y)) / dist)
            .x();
    double center_right_x =
        utils::DivideSegment(
            old_center, new_center,
            static_cast<double>(std::abs(std::min(dist, end_y - y + 1))) / dist)
            .x();
    for (double x = center_left_x + constants::kEps;; x += 1) {
      QPoint block_pos{static_cast<int>(std::floor(x)),
                       static_cast<int>(std::floor(y))};
      if (Model::GetInstance()->GetMap()->GetBlock(block_pos).GetType() !=
          Block::Type::kAir) {
        return false;
      }
      if (x >= std::floor(center_right_x)) {
        break;
      }
    }
  }
  return true;
}

bool Controller::CanAttackMob(std::shared_ptr<MovingObject> mob,
                              QPointF player_center, double lower_angle,
                              double upper_angle) const {
  QPointF mob_point = mob->GetPosition() + mob->GetSize() / 2 - player_center;
  if (Model::GetInstance()->GetPlayer()->IsAttackDirectionLeft()) {
    mob_point *= -1;
  }
  double distance = std::hypot(mob_point.x(), mob_point.y());
  double angle = std::atan2(mob_point.y(), mob_point.x());
  if (distance < constants::kEps) {
    return true;
  }
  return angle >= lower_angle && angle <= upper_angle &&
         distance <= constants::kPlayerAttackRadius &&
         IsVisible(player_center, mob_point + player_center);
}

void Controller::PlayerAttack() {
  Model::GetInstance()->GetPlayer()->DecAttackCooldownInterval();
  if (Model::GetInstance()->GetPlayer()->IsAttackFinished()) {
    return;
  }
  Model::GetInstance()->GetPlayer()->DecAttackTick();
  int attack_tick = Model::GetInstance()->GetPlayer()->GetAttackTick();
  double lower_angle = constants::kPlayerLowerAttackAngle +
                       constants::kPlayerAngleTick * attack_tick;
  double upper_angle = constants::kPlayerLowerAttackAngle +
                       (constants::kPlayerAngleTick) * (attack_tick + 1);
  QPointF player_center = Model::GetInstance()->GetPlayer()->GetPosition() +
                          Model::GetInstance()->GetPlayer()->GetSize() / 2;
  for (auto mob : Model::GetInstance()->GetMobs()) {
    if (CanAttackMob(mob, player_center, lower_angle, upper_angle)) {
      Damage damage(Model::GetInstance()->GetPlayer()->GetPosition(),
                    Damage::Type::kPlayer,
                    Model::GetInstance()->GetPlayer()->GetDamage());
      mob->DealDamage(damage);
    }
  }
}

void Controller::TickEvent() {
  Model::GetInstance()->MoveObjects(pressed_keys_);
  if (is_pressed_right_mouse_button) {
    BreakBlock();
    StartAttack();
  }
  PlayerAttack();
  View::GetInstance()->repaint();
}

ControllerTypes::Key Controller::TranslateKeyCode(int key_code) {
  switch (key_code) {
    case Qt::Key::Key_Left:
      return ControllerTypes::Key::kLeft;
    case Qt::Key::Key_Right:
      return ControllerTypes::Key::kRight;
    case Qt::Key::Key_Space:
      return ControllerTypes::Key::kJump;
    default:
      return ControllerTypes::Key::kUnused;
  }
}

void Controller::KeyPress(int key) {
  pressed_keys_.insert(TranslateKeyCode(key));
}

void Controller::KeyRelease(int key) {
  pressed_keys_.erase(TranslateKeyCode(key));
}

void Controller::PickItemToPlayer(InventoryItem item) {
  Model::GetInstance()->PickItemToPlayer(item);
}

void Controller::ButtonPress(Qt::MouseButton button) {
  if (button == Qt::MouseButton::LeftButton) {
    is_pressed_right_mouse_button = true;
  }
}

void Controller::ButtonRelease(Qt::MouseButton button) {
  if (button == Qt::MouseButton::LeftButton) {
    is_pressed_right_mouse_button = false;
  }
}
