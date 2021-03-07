#include "moving_object.h"

#include "model.h"

MovingObject::MovingObject(QPointF pos, double size_x, double size_y,
                           double walk_acceleration, double walk_max_speed,
                           double gravity_speed, double jump_speed, State state,
                           int state_ticks, MoveVector move_vector)
    : pos_(pos),
      size_x_(size_x),
      size_y_(size_y),
      walk_acceleration_(walk_acceleration),
      walk_max_speed_(walk_max_speed),
      gravity_speed_(gravity_speed),
      jump_speed_(jump_speed),
      state_(state),
      state_ticks_(state_ticks),
      move_vector_(move_vector) {
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

void MovingObject::Move(const std::unordered_set<int>& pressed_keys) {
  UpdateState(pressed_keys);
}

void MovingObject::UpdateState(const std::unordered_set<int>& pressed_keys) {
  std::shared_ptr<const Map>(Model::GetInstance()->GetMap());
  if (pressed_keys.find(Qt::Key::Key_Right) != pressed_keys.end()) {
    move_vector_.TranslateSpeed(walk_acceleration_, 0, walk_max_speed_,
                                MovingObject::kAbsoluteMaxSpeedY);
  } else {
    move_vector_.SetSpeed(0, 0);
  }
  UpdatePhysics();
}

void MovingObject::UpdatePhysics() {
  pos_ += (QPointF{move_vector_.GetSpeedX(), move_vector_.GetSpeedY()});
  qDebug() << pos_;
}
