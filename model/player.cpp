#include "model/player.h"

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

void Player::Read(const QJsonObject& json) {
  MovingObject::Read(json);

  inventory_->Read(json["inventory"].toObject());
  attack_tick_ = json["attack_tick_"].toDouble();
  attack_cooldown_interval_ = json["attack_cooldown_interval_"].toDouble();
  attack_direction_ =
      static_cast<utils::Direction>(json["attack_direction_"].toInt());
}

void Player::Write(QJsonObject& json) const {
  MovingObject::Write(json);

  QJsonObject inventory;
  inventory_->Write(inventory);
  json["inventory"] = inventory;
  json["attack_tick_"] = attack_tick_;
  json["attack_cooldown_interval_"] = attack_cooldown_interval_;
  json["attack_direction_"] = static_cast<int>(attack_direction_);
}
