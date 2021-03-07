#ifndef MODEL_PLAYER_H_
#define MODEL_PLAYER_H_

#include <QPoint>

#include "model/moving_object.h"

class Player : public MovingObject {
 public:
  Player(QPointF pos);

 private:
  // TODO(Wind-Eagle): make QPointF
  static constexpr double kSizeX = 0.75;
  static constexpr double kSizeY = 1.75;
  static constexpr double kWalkAcceleration = 0.01;
  static constexpr double kWalkMaxSpeed = 0.1;
  static constexpr double kWalkAirAcceleration = kWalkAcceleration;
  static constexpr double kWalkMaxAirAcceleration = kWalkMaxSpeed * 0.5;
  static constexpr double kGravitySpeed = 0.01;
  static constexpr double kJumpSpeed = -0.3;
};

#endif  // MODEL_PLAYER_H_
