#ifndef MODEL_ABSTRACT_STRATEGY_H_
#define MODEL_ABSTRACT_STRATEGY_H_

#include <QJsonObject>
#include <QPointF>
#include <unordered_set>

#include "controller/controller_types.h"

class MobState {
 public:
  MobState() = default;

  MobState(const MobState&) = default;
  MobState(MobState&&) = default;
  virtual ~MobState() = default;
  MobState& operator=(const MobState&) = default;
  MobState& operator=(MobState&&) = default;

  void SetPos(QPointF pos) { pos_ = pos; }
  void SetSize(QPointF size) { size_ = size; }
  void SetDamageAcceleration(QPointF damage_acceleration) {
    damage_acceleration_ = damage_acceleration;
  }
  void SetJump(QPoint jump) { jump_ = jump; }
  void SetDamageTime(double damage_time) { damage_time_ = damage_time; }
  void SetDamage(int damage) { damage_ = damage; }
  void SetGroundTouch(bool on_ground) { on_ground_ = on_ground; }
  void SetCeilTouch(bool on_ceil) { on_ceil_ = on_ceil; }
  void SetLeftTouch(bool pushes_left) { pushes_left_ = pushes_left; }
  void SetRightTouch(bool pushes_right) { pushes_right_ = pushes_right; }

  QPointF GetPos() const { return pos_; }
  QPointF GetSize() const { return size_; }
  QPointF GetDamageAcceleration() const { return damage_acceleration_; }
  QPoint GetJump() const { return jump_; }
  double GetDamageTime() const { return damage_time_; }
  int GetDamage() const { return damage_; }
  bool IsOnGround() const { return on_ground_; }
  bool IsOnCeil() const { return on_ceil_; }
  bool IsPushesLeft() const { return pushes_left_; }
  bool IsPushesRight() const { return pushes_right_; }

  void Read(const QJsonObject& json) {
    pos_.setX(json["pos_x"].toDouble());
    pos_.setY(json["pos_y"].toDouble());
    size_.setX(json["size_x"].toDouble());
    size_.setY(json["size_y"].toDouble());
    damage_acceleration_.setX(json["damage_acceleration_x"].toDouble());
    damage_acceleration_.setY(json["damage_acceleration_y"].toDouble());
    jump_.setX(json["jump_x"].toDouble());
    jump_.setY(json["jump_y"].toDouble());

    damage_time_ = json["damage_time"].toDouble();
    damage_ = json["damage"].toInt();
    on_ground_ = json["on_ground"].toBool();
    on_ceil_ = json["on_ceil_"].toBool();
    pushes_left_ = json["pushes_left_"].toBool();
    pushes_right_ = json["pushes_right_"].toBool();
  }

  void Write(QJsonObject* json) const {
    (*json)["pos_x"] = pos_.x();
    (*json)["pos_y"] = pos_.y();
    (*json)["size_x"] = size_.x();
    (*json)["size_y"] = size_.y();
    (*json)["damage_acceleration_x"] = damage_acceleration_.x();
    (*json)["damage_acceleration_y"] = damage_acceleration_.y();
    (*json)["jump_x"] = jump_.x();
    (*json)["jump_y"] = jump_.y();
    (*json)["damage_time"] = damage_time_;
    (*json)["damage"] = damage_;
    (*json)["on_ground"] = on_ground_;
    (*json)["on_ceil_"] = on_ceil_;
    (*json)["pushes_left_"] = pushes_left_;
    (*json)["pushes_right_"] = pushes_right_;
  }

 private:
  QPointF pos_ = {0, 0};
  QPointF size_ = {0, 0};
  QPointF damage_acceleration_ = {0.01, -0.015};
  QPoint jump_ = {2, 2};
  double damage_time_ = 0;
  int damage_ = 10;
  bool on_ground_ = false;
  bool on_ceil_ = false;
  bool pushes_left_ = false;
  bool pushes_right_ = false;
};

class AbstractStrategy {
 public:
  AbstractStrategy() = default;
  AbstractStrategy(const AbstractStrategy&) = default;
  AbstractStrategy(AbstractStrategy&&) = default;

  virtual ~AbstractStrategy() = default;

  AbstractStrategy& operator=(const AbstractStrategy&) = default;
  AbstractStrategy& operator=(AbstractStrategy&&) = default;

  virtual void Update(double time) = 0;

  virtual const std::unordered_set<ControllerTypes::Key>& GetKeys() const = 0;
  void SetMobState(const MobState& mob_state) { mob_state_ = mob_state; }
  const MobState& GetMobState() const { return mob_state_; }

  virtual void Read(const QJsonObject& json) {
    mob_state_.Read(json["mob_state"].toObject());
  }

  virtual void Write(QJsonObject* json) const {
    QJsonObject mob_state;
    mob_state_.Write(&mob_state);
    (*json)["mob_state"] = mob_state;
  }

 private:
  MobState mob_state_;
};

#endif  // MODEL_ABSTRACT_STRATEGY_H_
