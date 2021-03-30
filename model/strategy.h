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
  void SelectNewState() override;
  void UpdateConditions() override;
  bool IsActionFinished() override;
  void MakeAction() override;

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

#endif  // MODEL_STRATEGY_H_
