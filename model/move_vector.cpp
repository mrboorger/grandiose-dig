#include "move_vector.h"

#include <algorithm>

MoveVector::MoveVector(double speedX, double speedY, double momentumX,
                       double momentumY)
    : speed_(speedX, speedY), momentum_(momentumX, momentumY) {}

void MoveVector::TranslateSpeedX(double x, double left_limit,
                                 double right_limit) {
  QPointF new_speed = speed_ + QPointF(x, 0);
  if (left_limit <= new_speed.x() && right_limit >= new_speed.x()) {
    speed_ = new_speed;
  } else if (left_limit > new_speed.x() && x > 0) {
    speed_ = new_speed;
  } else if (right_limit < new_speed.x() && x < 0) {
    speed_ = new_speed;
  }
}

void MoveVector::TranslateSpeedY(double y, double left_limit,
                                 double right_limit) {
  QPointF new_speed = speed_ + QPointF(0, y);
  if (left_limit <= new_speed.y() && right_limit >= new_speed.y()) {
    speed_ = new_speed;
  } else if (left_limit > new_speed.y() && y > 0) {
    speed_ = new_speed;
  } else if (right_limit < new_speed.y() && y < 0) {
    speed_ = new_speed;
  }
}

void MoveVector::TranslateSpeedWithLimits(double x, double y) {
  TranslateSpeed(x, y);
  speed_.setX(std::clamp(speed_.x(), -constants::kAbsoluteMaxSpeedX,
                         constants::kAbsoluteMaxSpeedX));
  speed_.setY(std::clamp(speed_.y(), -constants::kAbsoluteMaxSpeedY,
                         constants::kAbsoluteMaxSpeedY));
}
