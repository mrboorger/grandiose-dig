#include "model/player.h"

#include "controller/controller.h"
#include "model/constants.h"

Player::Player(QPointF pos)
    : MovingObject(pos, constants::kPlayerSize), inventory_(new Inventory) {
  SetGravitySpeed(constants::kPlayerGravitySpeed);
  SetJumpSpeed(constants::kPlayerJumpSpeed);
  SetWalkAcceleration(constants::kPlayerWalkAcceleration);
  SetWalkAirAcceleration(constants::kPlayerWalkAirAcceleration);
  SetWalkMaxAirAcceleration(constants::kPlayerWalkMaxAirAcceleration);
  SetWalkMaxSpeed(constants::kPlayerWalkMaxSpeed);
  SetDamageAcceleration(constants::kPlayerDamageAcceleration);
  SetHealth(constants::kPlayerHealth);
  SetDamage(constants::kPlayerDamage);
  SetType(MovingObject::Type::kPlayer);
}

void Player::UseItem() { inventory_->RemoveOneSelectedItem(); }

bool Player::IsBlockReachableForTool(QPoint block_coords) {
  return std::hypot((GetPosition().x() - block_coords.x()),
                    (GetPosition().y() - block_coords.y())) <= ToolRadius;
}

bool Player::CanStartAttack() const {
  return attack_cooldown_interval_ <= constants::kEps;
}

void Player::SetUseItemCooldownInterval() {
  use_item_cooldown_interval = kUseItemCooldown;
}

void Player::DecItemUsingCooldownInterval(double time) {
  use_item_cooldown_interval = std::max(use_item_cooldown_interval - time, 0.0);
}
