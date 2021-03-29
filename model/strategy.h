#ifndef MODEL_STRATEGY_H
#define MODEL_STRATEGY_H

#include <QPointF>
#include <memory>
#include <vector>

#include "model/abstract_strategy.h"
#include "model/moving_object.h"

class BasicStrategy : public AbstractStrategy {
 public:
  BasicStrategy();
  virtual void Update() override;
  virtual void SelectNewState() override;
  virtual void UpdateConditions() override;
  virtual bool IsActionFinished() override;
  virtual void MakeAction() override;

 protected:
  void DecreaseIntervals();
  std::shared_ptr<const MovingObject> EnemySpotted();
  QPointF ChooseRandomWalkPosition() const;
  void DoStay();
  void DoWalk();
  void DoAttack();

 private:
  enum class State { kStay, kWalk, kAttack, kStatesCount };
  static constexpr int kStatesCount = static_cast<int>(State::kStatesCount);

  enum class Condition { kSeeEnemy, kCanAttack, kConditionsCount };
  static constexpr int kConditionsCount =
      static_cast<int>(Condition::kConditionsCount);
  State state_;
  std::vector<Condition> conditions_;
  std::shared_ptr<const MovingObject> attack_target_;
  QPointF walk_target_;
};

#endif  // MODEL_STRATEGY_H
