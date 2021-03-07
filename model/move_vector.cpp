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
QPointF MoveVector::GetSpeed() const { return speed_; }
double MoveVector::GetSpeedX() const { return speed_.x(); }
double MoveVector::GetSpeedY() const { return speed_.y(); }
QPointF MoveVector::GetMomentum() const { return momentum_; }
double MoveVector::GetMomentumX() const { return momentum_.x(); }
double MoveVector::GetMomentumY() const { return momentum_.y(); }
