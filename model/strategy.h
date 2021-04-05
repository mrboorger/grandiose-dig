#ifndef MODEL_STRATEGY_H_
#define MODEL_STRATEGY_H_

#include <QPointF>
#include <memory>
#include <vector>

#include "model/abstract_strategy.h"
#include "model/moving_object.h"

class BasicStrategy : public AbstractStrategy {
 public:
  BasicStrategy();
  void Update() override;
  virtual void SelectNewState();
  virtual void UpdateConditions();
  virtual bool IsActionFinished();
  virtual void PerformAction();
  virtual ~BasicStrategy() = default;
  const std::unordered_set<ControllerTypes::Key>& GetKeys() const override {
    return keys_;
  }

 protected:
  void DecreaseIntervals();
  std::shared_ptr<const MovingObject> EnemySpotted();
  QPointF ChooseRandomWalkPosition() const;
  void DoStay();
  void DoWalk();
  void DoAttack();

  void UpdateStay();
  void UpdateWalk();
  void UpdateAttack();

 private:
  enum class State { kStay, kWalk, kAttack, kStatesCount };
  enum class Condition { kSeeEnemy, kCanAttack, kConditionsCount };

  static constexpr int kStatesCount = static_cast<int>(State::kStatesCount);
  static constexpr int kConditionsCount =
      static_cast<int>(Condition::kConditionsCount);

  bool HasCondition(Condition condition) const {
    return (conditions_ & (1 << static_cast<uint32_t>(condition))) != 0;
  }
  void AddCondition(Condition condition) {
    conditions_ |= 1 << static_cast<uint32_t>(condition);
  }
  void RemoveCondition(Condition condition) {
    conditions_ &= ~(1 << static_cast<uint32_t>(condition));
  }
  void ClearConditions() { conditions_ = 0; }

  std::unordered_set<ControllerTypes::Key> keys_;

  int attack_interval_ = 0;
  int walk_interval_ = 0;
  QPointF walk_target_ = {0, 0};
  std::shared_ptr<const MovingObject> attack_target_;
  uint32_t conditions_;
  State state_;
};

#endif  // MODEL_STRATEGY_H_
