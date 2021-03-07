#ifndef MODEL_MOVE_VECTOR_H_
#define MODEL_MOVE_VECTOR_H_

#include <QPoint>

class MoveVector {
 public:
  MoveVector() = default;
  MoveVector(QPointF speed, QPointF momentum, bool is_momentum_locked = false);
  MoveVector(double speedX, double speedY, double momentumX, double momentumY);

  void TranslateSpeed(QPointF point);
  void TranslateSpeed(double x, double y);
  void SetSpeed(QPointF point);
  void SetSpeed(double x, double y);

  void TranslateSpeed(QPointF point, QPointF max_speed);
  void TranslateSpeed(double x, double y, double max_speed_x,
                      double max_speed_y);
  void SetSpeed(QPointF point, QPointF max_speed);
  void SetSpeed(double x, double y, double max_speed_x, double max_speed_y);

  void SetSpeedX(double x);
  void SetSpeedY(double y);
  void SetSpeedX(double x, double max_speed_x);
  void SetSpeedY(double y, double max_speed_y);

  void SetMomentum(QPointF momentum);
  void SetMomentum(double x, double y);
  void ResetMomentum();

  QPointF GetSpeed() const;
  double GetSpeedX() const;
  double GetSpeedY() const;
  QPointF GetMomentum() const;
  double GetMomentumX() const;
  double GetMomentumY() const;

 private:
  QPointF speed_;
  QPointF momentum_;
};

#endif  // MODEL_MOVE_VECTOR_H_
