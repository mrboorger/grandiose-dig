#include "player.h"

Player::Player(QPointF pos)
    : MovingObject(MoveVector(0, 0, 0, 0), pos, Player::kSize,
                   MovingObject::State::kStay, Player::kWalkAcceleration,
                   Player::kWalkMaxSpeed, Player::kWalkAirAcceleration,
                   Player::kWalkMaxAirAcceleration, Player::kGravitySpeed,
                   Player::kJumpSpeed) {}
