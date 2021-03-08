#ifndef MODEL_PLAYER_H_
#define MODEL_PLAYER_H_

#include <QPoint>

#include "model/moving_object.h"

class Player : public MovingObject {
 public:
  explicit Player(QPointF pos);

 private:
  // TODO(Wind-Eagle): make QPointF
  static constexpr QPointF kSize = {0.75, 1.75};
  static constexpr double kWalkAcceleration = 0.01;
  static constexpr double kWalkMaxSpeed = 0.1;
  static constexpr double kWalkAirAcceleration = kWalkAcceleration;
  static constexpr double kWalkMaxAirAcceleration = kWalkMaxSpeed;
  static constexpr double kGravitySpeed = 0.01;
  static constexpr double kJumpSpeed = -0.3;
};

#endif  // MODEL_PLAYER_H_