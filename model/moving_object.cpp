#include "moving_object.h"

#include "model.h"

MovingObject::MovingObject(QPointF pos, double size_x, double size_y,
                           double walk_acceleration, double walk_max_speed,
                           double walk_air_acceleration,
                           double walk_max_air_acceleration,
                           double gravity_speed, double jump_speed, State state,
                           int state_ticks, MoveVector move_vector)
    : pos_(pos),
      size_x_(size_x),
      size_y_(size_y),
      walk_acceleration_(walk_acceleration),
      walk_max_speed_(walk_max_speed),
      walk_air_acceleration_(walk_air_acceleration),
      walk_max_air_acceleration_(walk_max_air_acceleration),
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

bool IsKey(const std::unordered_set<int>& set, int key) {
  return set.find(key) != set.end();
}

void MovingObject::UpdateState(const std::unordered_set<int>& pressed_keys) {
  QPointF old_position = pos_;
  switch (state_) {
    case State::kStay:
      move_vector_.SetSpeed({0, 0});
      if (!pushes_ground_) {
        state_ = State::kJump;
        move_vector_.SetMomentum(move_vector_.GetSpeed());
        move_vector_.SetSpeed({0, 0});
        state_ticks_ = 0;
        break;
      }
      if (IsKey(pressed_keys, Qt::Key::Key_Left) !=
          IsKey(pressed_keys, Qt::Key::Key_Right)) {
        state_ = State::kWalk;
        state_ticks_ = 0;
        break;
      } else if (IsKey(pressed_keys, Qt::Key::Key_Space)) {
        state_ = State::kJump;
        move_vector_.SetSpeedY(jump_speed_);
        move_vector_.SetMomentum(move_vector_.GetSpeed());
        move_vector_.SetSpeed({0, 0});
        state_ticks_ = 0;
        break;
      }
      state_ticks_++;
      break;
    case State::kWalk:
      if (IsKey(pressed_keys, Qt::Key::Key_Left) ==
          IsKey(pressed_keys, Qt::Key::Key_Right)) {
        state_ = State::kStay;
        state_ticks_ = 0;
        move_vector_.SetSpeed({0, 0});
      } else if (IsKey(pressed_keys, Qt::Key::Key_Right)) {
        if (pushes_right_) {
          move_vector_.SetSpeedX(0);
        } else {
          move_vector_.SetSpeedX(walk_max_speed_);
        }
      } else if (IsKey(pressed_keys, Qt::Key::Key_Left)) {
        if (pushes_left_) {
          move_vector_.SetSpeedX(0);
        } else {
          move_vector_.SetSpeedX(-walk_max_speed_);
        }
      }
      if (pushes_ground_) {
        if (IsKey(pressed_keys, Qt::Key::Key_Space)) {
          move_vector_.SetSpeedY(jump_speed_);
        }
      } else {
        state_ = State::kJump;
        move_vector_.SetMomentum(move_vector_.GetSpeed());
        move_vector_.SetSpeed({0, 0});
        state_ticks_ = 0;
        break;
      }
      state_ticks_++;
      break;
    case State::kJump:
      move_vector_.TranslateSpeed(
          {0, gravity_speed_},
          {MovingObject::kAbsoluteMaxSpeedX, MovingObject::kAbsoluteMaxSpeedY});
      if (IsKey(pressed_keys, Qt::Key::Key_Left) ==
          IsKey(pressed_keys, Qt::Key::Key_Right)) {
        move_vector_.TranslateSpeed({0, 0});
      } else if (IsKey(pressed_keys, Qt::Key::Key_Right)) {
        if (pushes_right_) {
          move_vector_.SetSpeedX(0);
        } else {
          move_vector_.TranslateSpeed(walk_air_acceleration_, 0,
                                      walk_max_air_acceleration_,
                                      MovingObject::kAbsoluteMaxSpeedY);
        }
      } else if (IsKey(pressed_keys, Qt::Key::Key_Left)) {
        if (pushes_left_) {
          move_vector_.SetSpeedX(0);
        } else {
          move_vector_.TranslateSpeed(-walk_air_acceleration_, 0,
                                      walk_max_air_acceleration_,
                                      MovingObject::kAbsoluteMaxSpeedY);
        }
      }
      if (pushes_ground_) {
        if (IsKey(pressed_keys, Qt::Key::Key_Left) ==
            IsKey(pressed_keys, Qt::Key::Key_Right)) {
          state_ = State::kStay;
          state_ticks_ = 0;
          move_vector_.SetSpeed(0, 0);
        } else {
          state_ = State::kWalk;
          state_ticks_ = 0;
          move_vector_.SetSpeedY(0);
        }
      }
      state_ticks_++;
      break;
  }
  if (state_ != State::kJump) {
    move_vector_.ResetMomentum();
  }
  UpdatePhysics(old_position);
}

void MovingObject::UpdatePhysics(QPointF old_position) {
  pos_ += (move_vector_.GetSpeed());
  pos_ += (move_vector_.GetMomentum());
  if (pos_.y() > 148.25) {
    pos_.setY(148.25);
  }
  if (pos_.y() == 148.25) {
    pushes_ground_ = true;
  } else {
    pushes_ground_ = false;
  }
  qDebug() << pos_ << " " << pushes_ground_ << " " << (state_ == State::kJump)
           << " " << move_vector_.GetSpeed() << " "
           << move_vector_.GetMomentum();
}
