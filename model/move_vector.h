#ifndef MODEL_MOVE_VECTOR_H_
#define MODEL_MOVE_VECTOR_H_

#include <QPoint>
#include <algorithm>

#include "model/constants.h"

class MoveVector {
 public:
  MoveVector() = default;
  MoveVector(double speedX, double speedY, double momentumX, double momentumY);

  void TranslateSpeed(QPointF point);
  void TranslateSpeed(double x, double y);
  void TranslateSpeedX(double x,
                       double left_limit = -constants::kAbsoluteMaxSpeedX,
                       double right_limit = constants::kAbsoluteMaxSpeedX);
  void TranslateSpeedY(double y,
                       double left_limit = -constants::kAbsoluteMaxSpeedY,
                       double right_limit = constants::kAbsoluteMaxSpeedY);
  void SetSpeed(QPointF point);
  void SetSpeed(double x, double y);

  void TranslateSpeedWithLimits(double x, double y);

  void SetSpeedX(double x);
  void SetSpeedY(double y);

  void SetMomentum(QPointF momentum);
  void SetMomentum(double x, double y);
  void SetMomentumX(double x);
  void SetMomentumY(double x);
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
