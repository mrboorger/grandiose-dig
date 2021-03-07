#ifndef MOVE_VECTOR_H
#define MOVE_VECTOR_H

#include <QPoint>

class MoveVector {
 public:
  MoveVector() = default;
  MoveVector(QPointF speed, QPointF momentum, bool is_momentum_locked = false);
  MoveVector(double speedX, double speedY, double momentumX, double momentumY,
             bool is_momentum_locked = false);

  void TranslateSpeed(QPointF point) { speed_ += point; }
  void TranslateSpeed(double x, double y) { speed_ += QPointF(x, y); }
  void SetSpeed(QPointF point) { speed_ = point; }
  void SetSpeed(double x, double y) { speed_ = QPointF(x, y); }
  QPointF GetSpeed() const;
  double GetSpeedX() const;
  double GetSpeedY() const;
  QPointF GetMomentum() const;
  double GetMomentumX() const;
  double GetMomentumY() const;

 private:
  QPointF speed_;
  QPointF momentum_;
  bool is_momentum_locked_;
};

#endif  // MOVE_VECTOR_H
