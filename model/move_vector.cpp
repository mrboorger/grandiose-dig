#include "move_vector.h"

MoveVector::MoveVector(double speedX, double speedY, double momentumX,
                       double momentumY)
    : speed_(QPointF{speedX, speedY}),
      momentum_(QPointF{momentumX, momentumY}) {}

void MoveVector::TranslateSpeed(QPointF point) { speed_ += point; }
void MoveVector::TranslateSpeed(double x, double y) { speed_ += QPointF(x, y); }
void MoveVector::SetSpeed(QPointF point) { speed_ = point; }
void MoveVector::SetSpeed(double x, double y) { speed_ = QPointF(x, y); }

void MoveVector::TranslateSpeedX(double x, double left_limit,
                                 double right_limit) {
  QPointF new_speed = speed_ + QPointF(x, 0);
  if (left_limit <= new_speed.x() && right_limit >= new_speed.x()) {
    speed_ = new_speed;
    return;
  }
  if (left_limit > new_speed.x() && x > 0) {
    speed_ = new_speed;
    return;
  }
  if (right_limit < new_speed.x() && x < 0) {
    speed_ = new_speed;
    return;
  }
}
void MoveVector::TranslateSpeedY(double y, double left_limit,
                                 double right_limit) {
  QPointF new_speed = speed_ + QPointF(0, y);
  if (left_limit <= new_speed.y() && right_limit >= new_speed.y()) {
    speed_ = new_speed;
    return;
  }
  if (left_limit > new_speed.y() && y > 0) {
    speed_ = new_speed;
    return;
  }
  if (right_limit < new_speed.y() && y < 0) {
    speed_ = new_speed;
    return;
  }
}

void MoveVector::TranslateSpeedWithLimits(double x, double y) {
  TranslateSpeed(x, y);
  speed_.setX(std::min(speed_.x(), constants::kAbsoluteMaxSpeedX));
  speed_.setY(std::min(speed_.y(), constants::kAbsoluteMaxSpeedY));
  speed_.setX(std::max(speed_.x(), -constants::kAbsoluteMaxSpeedX));
  speed_.setY(std::max(speed_.y(), -constants::kAbsoluteMaxSpeedY));
}

void MoveVector::SetSpeedX(double x) { SetSpeed(x, speed_.y()); }

void MoveVector::SetSpeedY(double y) { SetSpeed(speed_.x(), y); }

void MoveVector::SetMomentum(QPointF momentum) { momentum_ = momentum; }
void MoveVector::SetMomentum(double x, double y) { momentum_ = {x, y}; }
void MoveVector::SetMomentumX(double x) { momentum_.setX(x); }
void MoveVector::SetMomentumY(double y) { momentum_.setY(y); }
void MoveVector::ResetMomentum() { momentum_ = {0, 0}; }

QPointF MoveVector::GetSpeed() const { return speed_; }
double MoveVector::GetSpeedX() const { return speed_.x(); }
double MoveVector::GetSpeedY() const { return speed_.y(); }
QPointF MoveVector::GetMomentum() const { return momentum_; }
double MoveVector::GetMomentumX() const { return momentum_.x(); }
double MoveVector::GetMomentumY() const { return momentum_.y(); }
