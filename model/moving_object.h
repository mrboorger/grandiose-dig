#ifndef MOVING_OBJECT_H
#define MOVING_OBJECT_H

#include <QImage>

#include "model/move_vector.h"

class MovingObject {
 public:
  enum class State { kStay, kWalk, kJump };

  void SetWalkAcceleration(double speed);
  void SetWalkMaxSpeed(double speed);
  void SetGravitySpeed(double speed);
  void SetJumpSpeed(double speed);

  double GetWalkAcceleration() const;
  double GetWalkMaxSpeed() const;
  double GetGravitySpeed() const;
  double GetJumpSpeed() const;

  QPointF GetPosition() const;

 protected:
  MovingObject(QPointF pos, double size_x, double size_y,
               double walk_acceleration, double walk_max_speed,
               double gravity_speed, double jump_speed,
               double jump_momentum = 0, State state = State::kStay);

 private:
  QPointF pos_;
  double size_x_;
  double size_y_;
  double walk_acceleration_;
  double walk_max_speed_;
  double gravity_speed_;
  double jump_speed_;
  double jump_momentum_;
  State state_;
  bool pushes_ground_;
  bool pushes_ceil_;
  bool pushes_right_;
  bool pushes_left_;
};

#endif  // MOVING_OBJECT_H
