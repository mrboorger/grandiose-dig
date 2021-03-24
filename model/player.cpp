#include "player.h"

#include "model/constants.h"

Player::Player(QPointF pos) : MovingObject(pos, constants::kPlayerSize) {
  SetGravitySpeed(constants::kPlayerGravitySpeed);
  SetJumpSpeed(constants::kPlayerJumpSpeed);
  SetWalkAcceleration(constants::kPlayerWalkAcceleration);
  SetWalkAirAcceleration(constants::kPlayerWalkAirAcceleration);
  SetWalkMaxAirAcceleration(constants::kPlayerWalkMaxAirAcceleration);
  SetWalkMaxSpeed(constants::kPlayerWalkMaxSpeed);
}
