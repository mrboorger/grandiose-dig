#include "controller/controller.h"

#include <QDebug>
#include <cmath>
#include <memory>

#include "model/abstract_map_generator.h"
#include "model/chunk_map.h"
#include "model/constants.h"
#include "view/abstract_map_drawer.h"
#include "view/buffered_map_drawer.h"
#include "view/map_drawer.h"

Controller* Controller::GetInstance() {
  static Controller controller;
  return &controller;
}

void Controller::SetGeneratedMap(AbstractMapGenerator* generator) {
  auto map = std::shared_ptr<AbstractMap>(generator->GenerateMap());
  Model::GetInstance()->SetMap(map);
  View::GetInstance()->SetDrawer(new BufferedMapDrawer(map));
}

Controller::Controller() : tick_timer_() {
  tick_timer_.callOnTimeout([this]() { TickEvent(); });
  tick_timer_.start(constants::kTickDurationMsec);
}

void Controller::SetPlayer() const {
  // TODO(Wind-Eagle): this is temporary code.
  // std::make_shared<Player>(QPointF(-3.0, 126.0)));
  Model::GetInstance()->SetPlayer(
      std::make_shared<Player>(QPointF(147.0, 126.0)));
  View::GetInstance()->SetInventoryDrawer(
      new InventoryDrawer(Model::GetInstance()->GetPlayer()->GetInventory()));
}

void Controller::SetMob() {
  // TODO(Wind-Eagle): this is temporary code.
  Model::GetInstance()->AddMob(
      std::make_shared<Mob>(QPointF(157.0, 126.0), QPointF(0.75, 1.75)));
}

void Controller::BreakBlock() {
  QPointF block_coord =
      View::GetInstance()->GetTopLeftWindowCoord() +
      QPointF(View::GetInstance()->GetCursorPos()) / constants::kBlockSz;
  QPoint block_coords_casted =
      QPoint(std::floor(block_coord.x()), std::floor(block_coord.y()));
  Model::GetInstance()->GetMap()->HitBlock(block_coords_casted, 1);
  View::GetInstance()->UpdateBlock(block_coords_casted);
}

void Controller::StartAttack() {
  int attack_interval = Model::GetInstance()->GetPlayer()->GetAttackInterval();
  if (attack_interval != 0) {
    return;
  }
  QPointF click_coord =
      View::GetInstance()->GetTopLeftWindowCoord() +
      QPointF(View::GetInstance()->GetCursorPos()) / constants::kBlockSz;
  if (click_coord.x() < Model::GetInstance()->GetPlayer()->GetPosition().x()) {
    Model::GetInstance()->GetPlayer()->SetAttackLeft(true);
  } else {
    Model::GetInstance()->GetPlayer()->SetAttackLeft(false);
  }
  Model::GetInstance()->GetPlayer()->SetAttackTick(
      constants::kPlayerAttackTime);
  Model::GetInstance()->GetPlayer()->SetAttackInterval(
      constants::kPlayerAttackCooldown);
}

namespace {
QPointF DivideSegment(QPointF first, QPointF second, double percentage) {
  return first * percentage + second * (1.0 - percentage);
}

bool IsVisible(double x1, double y1, double x2, double y2) {
  if (y1 < y2) {
    std::swap(x1, x2);
    std::swap(y1, y2);
  }
  QPointF old_center = {x1, y1};
  QPointF new_center = {x2, y2};
  int end_y = std::floor(new_center.y());
  int begin_y = std::floor(old_center.y());
  int dist = std::max(std::abs(end_y - begin_y), 1);
  for (int y = begin_y; y >= end_y; y--) {
    double center_left_x =
        DivideSegment(old_center, new_center,
                      static_cast<double>(std::abs(end_y - y)) / dist)
            .x();
    double center_right_x = center_left_x;
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

}  // namespace

void Controller::PlayerAttack() {
  Model::GetInstance()->GetPlayer()->DecAttackInterval();
  if (Model::GetInstance()->GetPlayer()->GetAttackTick() == 0) {
    return;
  }
  Model::GetInstance()->GetPlayer()->DecAttackTick();
  int attack_tick = Model::GetInstance()->GetPlayer()->GetAttackTick();
  double lower_angle = constants::kPlayerLowerAttackAngle +
                       constants::kPlayerAngleTick * attack_tick;
  double upper_angle = constants::kPlayerLowerAttackAngle +
                       (constants::kPlayerAngleTick) * (attack_tick + 1);
  double center_x = Model::GetInstance()->GetPlayer()->GetPosition().x() +
                    Model::GetInstance()->GetPlayer()->GetSize().x() / 2;
  double center_y = Model::GetInstance()->GetPlayer()->GetPosition().y() +
                    Model::GetInstance()->GetPlayer()->GetSize().y() / 2;
  for (auto i : Model::GetInstance()->GetMobs()) {
    double mob_x = i->GetPosition().x() + i->GetSize().x() / 2 - center_x;
    double mob_y = i->GetPosition().y() + i->GetSize().y() / 2 - center_y;
    if (Model::GetInstance()->GetPlayer()->IsAttackLeft()) {
      mob_x *= -1;
      mob_y *= -1;
    }
    double distance = std::hypot(mob_x, mob_y);
    double angle = std::atan2(mob_y, mob_x);
    if (angle >= lower_angle && angle <= upper_angle &&
        distance <= constants::kPlayerAttackRadius &&
        IsVisible(center_x, center_y, mob_x + center_x, mob_y + center_y)) {
      Damage damage(QPointF{center_x, center_y}, Damage::Type::kPlayer,
                    Model::GetInstance()->GetPlayer()->GetDamage());
      i->DealDamage(damage);
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
