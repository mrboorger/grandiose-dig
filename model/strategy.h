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
  virtual void Update();
  virtual void SelectNewState();
  virtual void UpdateConditions();
  virtual bool IsActionFinished();
  virtual void PerformAction();

 protected:
  void DecreaseIntervals();
  std::shared_ptr<const MovingObject> EnemySpotted();
  QPointF ChooseRandomWalkPosition() const;
  void DoStay();
  void DoWalk();
  void DoAttack();

  int state_interval_ = 0;
  int attack_interval_ = 0;
  int walk_interval_ = 0;
  QPointF walk_target_ = {0, 0};
  std::shared_ptr<const MovingObject> attack_target_;

 private:
  enum class State { kStay, kWalk, kAttack, kStatesCount };
  enum class Condition { kSeeEnemy, kCanAttack, kConditionsCount };

  static constexpr int kStatesCount = static_cast<int>(State::kStatesCount);
  static constexpr int kConditionsCount =
      static_cast<int>(Condition::kConditionsCount);

  bool HasCondition(Condition condition) {
    return std::find(conditions_.begin(), conditions_.end(), condition) !=
           conditions_.end();
  }

  std::vector<Condition> conditions_;
  State state_;
};

#endif  // MODEL_STRATEGY_H_
