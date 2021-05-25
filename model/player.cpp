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
                    (GetPosition().y() - block_coords.y())) <= tool_radius;
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

void Player::TryCraft(const CraftRecipe& recipe) {
  if (inventory_->CanCraft(recipe)) {
    inventory_->Craft(recipe);
  }
}

void Player::Read(const QJsonObject& json) {
  MovingObject::Read(json);

  inventory_->Read(json["inventory"].toObject());
  attack_tick_ = json["attack_tick_"].toDouble();
  attack_cooldown_interval_ = json["attack_cooldown_interval_"].toDouble();
  attack_direction_ =
      static_cast<utils::Direction>(json["attack_direction_"].toInt());
  use_item_cooldown_interval = json["use_item_cooldown_interval"].toInt();
  tool_radius = json["tool_radius"].toInt();
}

void Player::Write(QJsonObject* json) const {
  MovingObject::Write(json);

  QJsonObject inventory;
  inventory_->Write(&inventory);
  (*json)["inventory"] = inventory;
  (*json)["attack_tick_"] = attack_tick_;
  (*json)["attack_cooldown_interval_"] = attack_cooldown_interval_;
  (*json)["attack_direction_"] = static_cast<int>(attack_direction_);
  (*json)["use_item_cooldown_interval"] = use_item_cooldown_interval;
  (*json)["tool_radius"] = tool_radius;
}
