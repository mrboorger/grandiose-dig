#ifndef MODEL_PLAYER_H_
#define MODEL_PLAYER_H_

#include <QPoint>

#include "model/moving_object.h"

/*
 double size_x_;
  double size_y_;
  double walk_acceleration_;
  double walk_max_speed_;
  double gravity_speed_;
  double jump_speed_;
  double jump_momentum_;
*/

class Player : public MovingObject {
 public:
  Player(QPointF pos);

 private:
  static constexpr double kSizeX = 0.75;
  static constexpr double kSizeY = 1.75;
  static constexpr double kWalkAcceleration = 0.01;
  static constexpr double kWalkMaxSpeed = 0.04;
  static constexpr double kGravitySpeed = 0.1;
  static constexpr double kJumpSpeed = 0.4;
};

#endif  // MODEL_PLAYER_H_
