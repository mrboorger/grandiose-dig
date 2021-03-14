#include "player.h"

Player::Player(QPointF pos)
    : MovingObject(
          MoveVector(0, 0, 0, 0), pos, constants::kPlayerSize,
          MovingObject::State::kStay, constants::kPlayerWalkAcceleration,
          constants::kPlayerWalkMaxSpeed, constants::kPlayerWalkAirAcceleration,
          constants::kPlayerWalkMaxAirAcceleration,
          constants::kPlayerGravitySpeed, constants::kPlayerJumpSpeed) {}
