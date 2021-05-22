#ifndef MODEL_MOVE_VECTOR_H_
#define MODEL_MOVE_VECTOR_H_

#include <QJsonObject>
#include <QPoint>

#include "model/constants.h"

class MoveVector {
 public:
  MoveVector() = default;
  MoveVector(double speedX, double speedY, double momentumX, double momentumY);

  void TranslateSpeed(QPointF point) { speed_ += point; }
  void TranslateSpeed(double x, double y) { speed_ += QPointF(x, y); }
  void TranslateSpeedXIfNearerToBounds(
      double x, double left_limit = -constants::kAbsoluteMaxSpeedX,
      double right_limit = constants::kAbsoluteMaxSpeedX);
  void TranslateSpeedYIfNearerToBounds(
      double y, double left_limit = -constants::kAbsoluteMaxSpeedY,
      double right_limit = constants::kAbsoluteMaxSpeedY);
  void TranslateSpeedWithLimits(double x, double y);

  void SetSpeed(QPointF point) { speed_ = point; }
  void SetSpeed(double x, double y) { speed_ = QPointF(x, y); }
  void SetSpeedX(double x) { speed_.setX(x); }
  void SetSpeedY(double y) { speed_.setY(y); }
  void ResetSpeed() { speed_ = {0, 0}; }

  void SetMomentum(QPointF momentum) { momentum_ = momentum; }
  void SetMomentum(double x, double y) { momentum_ = QPointF(x, y); }
  void SetMomentumX(double x) { momentum_.setX(x); }
  void SetMomentumY(double y) { momentum_.setY(y); }
  void ResetMomentum() { momentum_ = {0, 0}; }

  QPointF GetSpeed() const { return speed_; }
  double GetSpeedX() const { return speed_.x(); }
  double GetSpeedY() const { return speed_.y(); }

  QPointF GetMomentum() const { return momentum_; }
  double GetMomentumX() const { return momentum_.x(); }
  double GetMomentumY() const { return momentum_.y(); }

  void Read(const QJsonObject& json);
  void Write(QJsonObject& json) const;

 private:
  QPointF speed_;
  QPointF momentum_;
};

#endif  // MODEL_MOVE_VECTOR_H_
