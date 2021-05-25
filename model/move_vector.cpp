#include "model/move_vector.h"

#include <algorithm>

MoveVector::MoveVector(double speedX, double speedY, double momentumX,
                       double momentumY)
    : speed_(speedX, speedY), momentum_(momentumX, momentumY) {}

void MoveVector::TranslateSpeedXIfNearerToBounds(double x, double left_limit,
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

void MoveVector::TranslateSpeedYIfNearerToBounds(double y, double left_limit,
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

void MoveVector::Read(const QJsonObject& json) {
  speed_.setX(json["speed_x"].toDouble());
  speed_.setY(json["speed_y"].toDouble());
  momentum_.setX(json["momentum_x"].toDouble());
  momentum_.setY(json["momentum_y"].toDouble());
}

void MoveVector::Write(QJsonObject* json) const {
  (*json)["speed_x"] = speed_.x();
  (*json)["speed_y"] = speed_.y();
  (*json)["momentum_x"] = momentum_.x();
  (*json)["momentum_y"] = momentum_.y();
}
