#include "player.h"

Player::Player(QPointF pos)
    : MovingObject(pos, Player::kSizeX, Player::kSizeY,
                   Player::kWalkAcceleration, Player::kWalkMaxSpeed,
                   Player::kWalkAirAcceleration,
                   Player::kWalkMaxAirAcceleration, Player::kGravitySpeed,
                   Player::kJumpSpeed) {}
