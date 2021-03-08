#include "moving_object.h"

#include "model.h"

MovingObject::MovingObject(QPointF pos, QPointF size, double walk_acceleration,
                           double walk_max_speed, double walk_air_acceleration,
                           double walk_max_air_acceleration,
                           double gravity_speed, double jump_speed, State state,
                           int state_ticks, MoveVector move_vector)
    : pos_(pos),
      size_(size),
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
  // TODO(Wind-Eagle): the longer you hold SPACE bar the higher you jump.
  switch (state_) {
    case State::kStay:
      if (!pushes_ground_) {
        state_ = State::kJump;
        move_vector_.SetMomentum(move_vector_.GetSpeed());
        move_vector_.SetSpeed({0, 0});
        state_ticks_ = 0;
        break;
      }
      move_vector_.SetSpeed({0, 0});
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
        move_vector_.SetSpeedX(0);
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
      move_vector_.TranslateSpeedWithLimits(0, gravity_speed_);
      if (IsKey(pressed_keys, Qt::Key::Key_Left) ==
          IsKey(pressed_keys, Qt::Key::Key_Right)) {
        move_vector_.TranslateSpeed({0, 0});
      } else if (IsKey(pressed_keys, Qt::Key::Key_Right)) {
        if (pushes_right_) {
          move_vector_.SetSpeedX(0);
        } else {
          // TODO(Wind-Eagle): acknowledge why this piece of code is working
          move_vector_.TranslateSpeedX(
              walk_acceleration_,
              -walk_max_air_acceleration_ + move_vector_.GetMomentumX(),
              walk_max_air_acceleration_ - move_vector_.GetMomentumX());
        }
      } else if (IsKey(pressed_keys, Qt::Key::Key_Left)) {
        if (pushes_left_) {
          move_vector_.SetSpeedX(0);
        } else {
          // TODO(Wind-Eagle): acknowledge why this piece of code is working
          move_vector_.TranslateSpeedX(
              -walk_acceleration_,
              -walk_max_air_acceleration_ - move_vector_.GetMomentumX(),
              walk_max_air_acceleration_ + move_vector_.GetMomentumX());
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

QPointF SegmentDivide(QPointF first, QPointF second, double percentage) {
  return QPointF(first.x() * percentage + second.x() * (1 - percentage),
                 first.y() * percentage + second.y() * (1 - percentage));
}

bool MovingObject::HasCollisionGround(QPointF old_position, double* ground_y,
                                      std::shared_ptr<const Map> map) const {
  QPointF old_bottom_left = old_position + QPointF(0, size_.y());
  QPointF new_bottom_left = pos_ + QPointF(0, size_.y());
  int end_y = floor(new_bottom_left.y());
  int begin_y =
      std::min(static_cast<int>(floor(old_bottom_left.y()) + 1), end_y);
  int dist = std::max(std::abs(end_y - begin_y), 1);
  for (int y = begin_y; y <= end_y; y++) {
    double bottom_left =
        SegmentDivide(old_bottom_left, new_bottom_left,
                      static_cast<double>(std::abs(end_y - y)) / dist)
            .x();
    double bottom_right = bottom_left + size_.x() - constants::kEPS;
    for (double x = bottom_left + constants::kEPS;; x += 1) {
      int block_x = floor(x);
      int block_y = floor(y);
      if (map->GetBlock(block_x, block_y).GetType() != Block::Type::kAir) {
        (*ground_y) = block_y - size_.y();
        return true;
      }
      if (x >= floor(bottom_right)) break;
    }
  }
  return false;
}

bool MovingObject::HasCollisionCeiling(QPointF old_position, double* ceiling_y,
                                       std::shared_ptr<const Map> map) const {
  QPointF old_top_left = old_position;
  QPointF new_top_left = pos_;
  int end_y = floor(new_top_left.y());
  int begin_y = std::max(static_cast<int>(floor(old_top_left.y()) - 1), end_y);
  int dist = std::max(std::abs(end_y - begin_y), 1);
  for (int y = begin_y; y >= end_y; y--) {
    double top_left =
        SegmentDivide(old_top_left, new_top_left,
                      static_cast<double>(std::abs(end_y - y)) / dist)
            .x();
    double top_right = top_left + size_.x() - constants::kEPS;
    for (double x = top_left + constants::kEPS;; x += 1) {
      int block_x = floor(x);
      int block_y = floor(y);
      if (map->GetBlock(block_x, block_y).GetType() != Block::Type::kAir) {
        (*ceiling_y) = block_y + 1;
        return true;
      }
      if (x >= floor(top_right)) break;
    }
  }
  return false;
}

bool MovingObject::HasCollisionLeft(QPointF old_position, double* left_wall_x,
                                    std::shared_ptr<const Map> map) const {
  QPointF old_bottom_left =
      old_position + QPointF(0, size_.y() - constants::kEPS);
  QPointF new_bottom_left = pos_ + QPointF(0, size_.y() - constants::kEPS);
  int end_x = floor(new_bottom_left.x());
  int begin_x =
      std::max(static_cast<int>(floor(old_bottom_left.x()) - 1), end_x);
  int dist = std::max(std::abs(end_x - begin_x), 1);
  for (int x = begin_x; x >= end_x; x--) {
    double bottom_left =
        SegmentDivide(old_bottom_left, new_bottom_left,
                      static_cast<double>(std::abs(end_x - x)) / dist)
            .y();
    double top_left = bottom_left - size_.y();
    for (double y = bottom_left;; y -= 1) {
      int block_x = floor(x);
      int block_y = floor(y);
      if (map->GetBlock(block_x, block_y).GetType() != Block::Type::kAir) {
        (*left_wall_x) = block_x + 1;
        return true;
      }
      if (block_y <= top_left) break;
    }
  }
  return false;
}

bool MovingObject::HasCollisionRight(QPointF old_position, double* rightWallX,
                                     std::shared_ptr<const Map> map) const {
  QPointF old_bottom_right =
      old_position +
      QPointF(size_.x() - constants::kEPS, size_.y() - constants::kEPS);
  QPointF new_bottom_right =
      pos_ + QPointF(size_.x() - constants::kEPS, size_.y() - constants::kEPS);
  int end_x = floor(new_bottom_right.x());
  int begin_x =
      std::min(static_cast<int>(floor(old_bottom_right.x()) + 1), end_x);
  int dist = std::max(std::abs(end_x - begin_x), 1);
  for (int x = begin_x; x <= end_x; x++) {
    double bottom_right =
        SegmentDivide(old_bottom_right, new_bottom_right,
                      static_cast<double>(std::abs(end_x - x)) / dist)
            .y();
    double top_right = bottom_right - size_.y();
    for (double y = bottom_right;; y -= 1) {
      int block_x = floor(x);
      int block_y = floor(y);
      if (map->GetBlock(block_x, block_y).GetType() != Block::Type::kAir) {
        (*rightWallX) = block_x - size_.x();
        return true;
      }
      if (block_y <= top_right) break;
    }
  }
  return false;
}

void MovingObject::UpdatePhysics(QPointF old_position) {
  std::shared_ptr<const Map> map = Model::GetInstance()->GetMap();
  double ground_y = 0, ceiling_y = 0;
  double right_wall_x = 0, left_wall_x = 0;
  pos_ += move_vector_.GetSpeed();
  pos_ += move_vector_.GetMomentum();
  if (move_vector_.GetSpeedX() + move_vector_.GetMomentumX() <=
          constants::kEPS &&
      HasCollisionLeft(old_position, &left_wall_x, map)) {
    if (old_position.x() + constants::kEPS >= left_wall_x) {
      pushes_left_ = true;
      pos_.setX(left_wall_x);
    }
    move_vector_.SetSpeedX(0);
    move_vector_.SetMomentumX(0);
  } else {
    pushes_left_ = false;
  }
  if (move_vector_.GetSpeedX() + move_vector_.GetMomentumX() >=
          -constants::kEPS &&
      HasCollisionRight(old_position, &right_wall_x, map)) {
    if (old_position.x() - constants::kEPS <= right_wall_x) {
      pushes_right_ = true;
      pos_.setX(right_wall_x);
    }
    move_vector_.SetSpeedX(0);
    move_vector_.SetMomentumX(0);
  } else {
    pushes_right_ = false;
  }
  if (move_vector_.GetSpeedY() + move_vector_.GetMomentumY() <=
          constants::kEPS &&
      HasCollisionCeiling(old_position, &ceiling_y, map)) {
    pushes_ceil_ = true;
    pos_.setY(ceiling_y);
    move_vector_.SetSpeedY(0);
    move_vector_.SetMomentumY(0);
  } else {
    pushes_ceil_ = false;
  }
  if (move_vector_.GetSpeedY() + move_vector_.GetMomentumY() >=
          -constants::kEPS &&
      HasCollisionGround(old_position, &ground_y, map)) {
    pushes_ground_ = true;
    pos_.setY(ground_y);
    move_vector_.SetSpeedY(0);
    move_vector_.SetMomentumY(0);
  } else {
    pushes_ground_ = false;
  }
}
