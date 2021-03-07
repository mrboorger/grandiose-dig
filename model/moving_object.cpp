#include "moving_object.h"

MovingObject::MovingObject(QPointF pos, double size_x, double size_y,
                           double walk_acceleration, double walk_max_speed,
                           double gravity_speed, double jump_speed,
                           double jump_momentum, State state)
    : pos_(pos),
      size_x_(size_x),
      size_y_(size_y),
      walk_acceleration_(walk_acceleration),
      walk_max_speed_(walk_max_speed),
      gravity_speed_(gravity_speed),
      jump_speed_(jump_speed),
      jump_momentum_(jump_momentum),
      state_(state) {
  pushes_ground_ = false;
  pushes_ceil_ = false;
  pushes_left_ = false;
  pushes_right_ = false;
}

void MovingObject::SetWalkAcceleration(double speed) {
  walk_acceleration_ = speed;
}
void MovingObject::SetWalkMaxSpeed(double speed) { walk_max_speed_ = speed; }
void MovingObject::SetGravitySpeed(double speed) { gravity_speed_ = speed; }
void MovingObject::SetJumpSpeed(double speed) { jump_speed_ = speed; }

double MovingObject::GetWalkAcceleration() const { return walk_acceleration_; }
double MovingObject::GetWalkMaxSpeed() const { return walk_max_speed_; }
double MovingObject::GetGravitySpeed() const { return gravity_speed_; }
double MovingObject::GetJumpSpeed() const { return jump_speed_; }

QPointF MovingObject::GetPosition() const { return pos_; }
