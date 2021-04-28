#ifndef MODEL_ABSTRACT_STRATEGY_H_
#define MODEL_ABSTRACT_STRATEGY_H_

#include <QPointF>
#include <unordered_set>

#include "controller/controller_types.h"

class MobState {
 public:
  explicit MobState(QPointF pos = {0, 0}, QPointF size = {0, 0},
                    QPointF damage_acceleration = {0.01, -0.015},
                    QPoint jump = {2, 2}, int damage = 10,
                    bool on_ground = false, bool on_ceil = false,
                    bool pushes_left = false, bool pushes_right = false)
      : pos_(pos),
        size_(size),
        damage_acceleration_(damage_acceleration),
        jump_(jump),
        damage_(damage),
        on_ground_(on_ground),
        on_ceil_(on_ceil),
        pushes_left_(pushes_left),
        pushes_right_(pushes_right) {}

  MobState(const MobState&) = default;
  MobState(MobState&&) = default;
  virtual ~MobState() = default;
  MobState& operator=(const MobState&) = default;
  MobState& operator=(MobState&&) = default;

  QPointF GetPos() const { return pos_; }
  QPointF GetSize() const { return size_; }
  QPointF GetDamageAcceleration() const { return damage_acceleration_; }
  QPoint GetJump() const { return jump_; }
  int GetDamage() const { return damage_; }
  bool IsOnGround() const { return on_ground_; }
  bool IsOnCeil() const { return on_ceil_; }
  bool IsPushesLeft() const { return pushes_left_; }
  bool IsPushesRight() const { return pushes_right_; }

 private:
  QPointF pos_;
  QPointF size_;
  QPointF damage_acceleration_;
  QPoint jump_;
  int damage_;
  bool on_ground_;
  bool on_ceil_;
  bool pushes_left_;
  bool pushes_right_;
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

 private:
  MobState mob_state_;
};

#endif  // MODEL_ABSTRACT_STRATEGY_H_
