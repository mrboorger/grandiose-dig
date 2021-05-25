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
                    (GetPosition().y() - block_coords.y())) <= tool_radius_;
}

bool Player::CanStartAttack() const {
  return attack_cooldown_interval_ <= constants::kEps;
}

void Player::SetUseItemCooldownInterval() {
  use_item_cooldown_interval_ = kUseItemCooldown;
}

void Player::DecUseItemCooldownInterval(double time) {
  use_item_cooldown_interval_ =
      std::max(use_item_cooldown_interval_ - time, 0.0);
}

void Player::TryCraft(const CraftRecipe& recipe) {
  if (inventory_->CanCraft(recipe)) {
    inventory_->Craft(recipe);
  }
}
void Player::UsePotion() {
  if (inventory_->GetSelectedItem().GetType() ==
      InventoryItem::Type::kSpeedPotion) {
    AddEffect(Effect(Effect::Type::kSpeed, 30000, 1.5));
  } else if (inventory_->GetSelectedItem().GetType() ==
             InventoryItem::Type::kSpeedPotion) {
    AddEffect(Effect(Effect::Type::kStrength, 30000, 1.5));
  }
  inventory_->RemoveOneSelectedItem();
}
