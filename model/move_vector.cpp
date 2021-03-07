#include "move_vector.h"

MoveVector::MoveVector(double speedX, double speedY, double momentumX,
                       double momentumY)
    : speed_(QPointF{speedX, speedY}),
      momentum_(QPointF{momentumX, momentumY}) {}

MoveVector::MoveVector(QPointF speed, QPointF momentum, bool is_momentum_locked)
    : speed_(speed), momentum_(momentum) {}

void MoveVector::TranslateSpeed(QPointF point) { speed_ += point; }
void MoveVector::TranslateSpeed(double x, double y) { speed_ += QPointF(x, y); }
void MoveVector::SetSpeed(QPointF point) { speed_ = point; }
void MoveVector::SetSpeed(double x, double y) { speed_ = QPointF(x, y); }

void MoveVector::TranslateSpeed(QPointF point, QPointF max_speed) {
  TranslateSpeed(point);
  speed_.setX(std::min(speed_.x(), max_speed.x()));
  speed_.setY(std::min(speed_.y(), max_speed.y()));
  speed_.setX(std::max(speed_.x(), -max_speed.x()));
  speed_.setY(std::max(speed_.y(), -max_speed.y()));
}
void MoveVector::TranslateSpeed(double x, double y, double max_speed_x,
                                double max_speed_y) {
  TranslateSpeed(x, y);
  speed_.setX(std::min(speed_.x(), max_speed_x));
  speed_.setY(std::min(speed_.y(), max_speed_y));
  speed_.setX(std::max(speed_.x(), -max_speed_x));
  speed_.setY(std::max(speed_.y(), -max_speed_y));
}
void MoveVector::SetSpeed(QPointF point, QPointF max_speed) {
  SetSpeed(point);
  speed_.setX(std::min(speed_.x(), max_speed.x()));
  speed_.setY(std::min(speed_.y(), max_speed.y()));
  speed_.setX(std::max(speed_.x(), -max_speed.x()));
  speed_.setY(std::max(speed_.y(), -max_speed.y()));
}
void MoveVector::SetSpeed(double x, double y, double max_speed_x,
                          double max_speed_y) {
  SetSpeed(x, y);
  speed_.setX(std::min(speed_.x(), max_speed_x));
  speed_.setY(std::min(speed_.y(), max_speed_y));
  speed_.setX(std::max(speed_.x(), -max_speed_x));
  speed_.setY(std::max(speed_.y(), -max_speed_y));
}

void MoveVector::SetSpeedX(double x) { SetSpeed(x, speed_.y()); }

void MoveVector::SetSpeedY(double y) { SetSpeed(speed_.x(), y); }

void MoveVector::SetSpeedX(double x, double max_speed_x) {
  SetSpeedX(x);
  speed_.setX(std::min(speed_.x(), max_speed_x));
  speed_.setX(std::max(speed_.x(), -max_speed_x));
}

void MoveVector::SetSpeedY(double y, double max_speed_y) {
  SetSpeedY(y);
  speed_.setY(std::min(speed_.y(), max_speed_y));
  speed_.setY(std::min(speed_.y(), -max_speed_y));
}

void MoveVector::SetMomentum(QPointF momentum) { momentum_ = momentum; }
void MoveVector::SetMomentum(double x, double y) { momentum_ = {x, y}; }
void MoveVector::ResetMomentum() { momentum_ = {0, 0}; }

QPointF MoveVector::GetSpeed() const { return speed_; }
double MoveVector::GetSpeedX() const { return speed_.x(); }
double MoveVector::GetSpeedY() const { return speed_.y(); }
QPointF MoveVector::GetMomentum() const { return momentum_; }
double MoveVector::GetMomentumX() const { return momentum_.x(); }
double MoveVector::GetMomentumY() const { return momentum_.y(); }
