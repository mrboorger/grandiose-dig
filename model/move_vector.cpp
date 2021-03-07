#include "move_vector.h"

MoveVector::MoveVector(double speedX, double speedY, double momentumX,
                       double momentumY, bool is_momentum_locked)
    : speed_(QPointF{speedX, speedY}),
      momentum_(QPointF{momentumX, momentumY}),
      is_momentum_locked_(is_momentum_locked) {}

MoveVector::MoveVector(QPointF speed, QPointF momentum, bool is_momentum_locked)
    : speed_(speed),
      momentum_(momentum),
      is_momentum_locked_(is_momentum_locked) {}

void MoveVector::TranslateSpeed(QPointF point) { speed_ += point; }
void MoveVector::TranslateSpeed(double x, double y) { speed_ += QPointF(x, y); }
void MoveVector::SetSpeed(QPointF point) { speed_ = point; }
void MoveVector::SetSpeed(double x, double y) { speed_ = QPointF(x, y); }

void MoveVector::TranslateSpeed(QPointF point, QPointF max_speed) {
  TranslateSpeed(point);
  speed_.setX(std::min(speed_.x(), max_speed.x()));
  speed_.setY(std::min(speed_.y(), max_speed.y()));
}
void MoveVector::TranslateSpeed(double x, double y, double max_speed_x,
                                double max_speed_y) {
  TranslateSpeed(x, y);
  speed_.setX(std::min(speed_.x(), max_speed_x));
  speed_.setY(std::min(speed_.y(), max_speed_y));
}
void MoveVector::SetSpeed(QPointF point, QPointF max_speed) {
  SetSpeed(point);
  speed_.setX(std::min(speed_.x(), max_speed.x()));
  speed_.setY(std::min(speed_.y(), max_speed.y()));
}
void MoveVector::SetSpeed(double x, double y, double max_speed_x,
                          double max_speed_y) {
  SetSpeed(x, y);
  speed_.setX(std::min(speed_.x(), max_speed_x));
  speed_.setY(std::min(speed_.y(), max_speed_y));
}

QPointF MoveVector::GetSpeed() const { return speed_; }
double MoveVector::GetSpeedX() const { return speed_.x(); }
double MoveVector::GetSpeedY() const { return speed_.y(); }
QPointF MoveVector::GetMomentum() const { return momentum_; }
double MoveVector::GetMomentumX() const { return momentum_.x(); }
double MoveVector::GetMomentumY() const { return momentum_.y(); }
