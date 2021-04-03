#ifndef MODEL_ABSTRACT_STRATEGY_H_
#define MODEL_ABSTRACT_STRATEGY_H_

#include <QPointF>
#include <unordered_set>

#include "controller/controller_types.h"

class MobState {
 public:
  explicit MobState(QPointF pos = {0, 0}, QPointF size = {0, 0},
                    bool on_ground = false, bool on_ceil = false,
                    bool pushes_left = false, bool pushes_right = false)
      : pos_(pos),
        size_(size),
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
  bool IsOnGround() const { return on_ground_; }
  bool IsOnCeil() const { return on_ceil_; }
  bool IsPushesLeft() const { return pushes_left_; }
  bool IsPushesRight() const { return pushes_right_; }

 private:
  QPointF pos_;
  QPointF size_;
  bool on_ground_;
  bool on_ceil_;
  bool pushes_left_;
  bool pushes_right_;
};

class AbstractStrategy {
 public:
  virtual void Update() = 0;
  virtual void SelectNewState() = 0;
  virtual void UpdateConditions() = 0;
  virtual bool IsActionFinished() = 0;
  virtual void MakeAction() = 0;

  virtual const std::unordered_set<ControllerTypes::Key> GetKeys() {
    return keys_;
  }
  void SetMobState(const MobState& mob_state) { mob_state_ = mob_state; }

 protected:
  MobState mob_state_;
  std::unordered_set<ControllerTypes::Key> keys_;
  int state_interval_ = 0;
  int attack_interval_ = 0;
  int walk_interval_ = 0;
};

#endif  // MODEL_ABSTRACT_STRATEGY_H_