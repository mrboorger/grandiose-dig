#ifndef MODEL_STRATEGY_H_
#define MODEL_STRATEGY_H_

#include <QJsonArray>
#include <QPointF>
#include <memory>
#include <unordered_set>
#include <vector>

#include "model/abstract_strategy.h"
#include "model/moving_object.h"
#include "utils.h"

class BasicStrategy : public AbstractStrategy {
 public:
  BasicStrategy();
  BasicStrategy(const BasicStrategy& strategy) = default;
  BasicStrategy(BasicStrategy&& strategy) = default;

  virtual ~BasicStrategy() = default;

  BasicStrategy& operator=(const BasicStrategy& strategy) = default;
  BasicStrategy& operator=(BasicStrategy&& strategy) = default;

  void Update(double time) override;
  virtual void SelectNewState();
  virtual void UpdateConditions();
  virtual bool IsActionFinished();
  virtual void PerformAction();

  const std::unordered_set<ControllerTypes::Key>& GetKeys() const override {
    return keys_;
  }

  void Read(const QJsonObject& json) override {
    AbstractStrategy::Read(json);

    vision_radius_ = json["vision_radius"].toDouble();
    walk_time_count_ = json["walk_time_count"].toDouble();
    attack_time_count_ = json["attack_time_count"].toDouble();
    walk_precision_ = json["walk_precision"].toDouble();
    random_walk_chance_ = json["random_walk_chance"].toDouble();
    random_walk_distance_ = json["random_walk_distance"].toDouble();

    attack_interval_ = json["attack_interval"].toDouble();
    walk_interval_ = json["walk_interval"].toDouble();
    walk_target_.setX(json["walk_target_x"].toDouble());
    walk_target_.setY(json["walk_target_y"].toDouble());
    conditions_ = json["conditions"].toInt();
    state_ = static_cast<State>(json["state"].toInt());
  }

  void Write(QJsonObject& json) const override {
    AbstractStrategy::Write(json);
    json["strategy_type"] = "BasicStrategy";

    json["vision_radius"] = vision_radius_;
    json["walk_time_count"] = walk_time_count_;
    json["attack_time_count"] = attack_time_count_;
    json["walk_precision"] = walk_precision_;
    json["random_walk_chance"] = random_walk_chance_;
    json["random_walk_distance"] = random_walk_distance_;

    json["attack_interval"] = attack_interval_;
    json["walk_interval"] = walk_interval_;
    json["walk_target_x"] = walk_target_.x();
    json["walk_target_y"] = walk_target_.y();
    json["conditions"] = static_cast<int>(conditions_);
    json["state"] = static_cast<int>(state_);
  }

 protected:
  virtual void DecreaseIntervals(double times);
  std::shared_ptr<MovingObject> EnemySpotted();
  QPointF ChooseRandomWalkPosition() const;

  virtual void DoStay();
  virtual void DoWalk();
  virtual void DoAttack();

  virtual void DoWalkActions();

  void UpdateStay();
  void UpdateWalk();
  void UpdateAttack();

  bool IsNearPit(QPointF src, utils::Direction side) const;

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
  virtual bool AlmostNearX(QPointF lhs, QPointF rhs);

  std::unordered_set<ControllerTypes::Key> keys_;

  double vision_radius_;
  double walk_time_count_;
  double attack_time_count_;
  double walk_precision_;
  double random_walk_chance_;
  double random_walk_distance_;

  double attack_interval_ = 0;
  double walk_interval_ = 0;
  QPointF walk_target_ = {0, 0};
  std::shared_ptr<MovingObject> attack_target_;
  uint32_t conditions_;
  State state_;
};

class ZombieSummonerStrategy : public BasicStrategy {
 public:
  ZombieSummonerStrategy();

  void Read(const QJsonObject& json) override {
    BasicStrategy::Read(json);

    summon_interval = json["summon_interval"].toDouble();
  }

  void Write(QJsonObject& json) const override {
    BasicStrategy::Write(json);

    json["strategy_type"] = "ZombieSummonerStrategy";
    json["summon_interval"] = summon_interval;
  }

 protected:
  void DecreaseIntervals(double times) override;
  void DoWalkActions() override;
  void SummonZombie();

  double summon_interval = 0;
};

class MagicStrategy : public BasicStrategy {
 public:
  MagicStrategy();

  void Read(const QJsonObject& json) override {
    BasicStrategy::Read(json);
  }

  void Write(QJsonObject& json) const override {
    BasicStrategy::Write(json);

    json["strategy_type"] = "MagicStrategy";
  }

 protected:
  void DoAttack() override;
};

#endif  // MODEL_STRATEGY_H_
