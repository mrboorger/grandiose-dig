#include "model/strategy.h"

#include <QDebug>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <memory>
#include <random>

#include "model/model.h"
#include "utils.h"

BasicStrategy::BasicStrategy() {
  vision_radius_ = constants::kBasicStrategyVisionRadius;
  walk_time_count_ = constants::kBasicStrategyWalkTimeCount;
  attack_time_count_ = constants::kBasicStrategyAttackTimeCount;
  walk_precision_ = constants::kBasicStrategyWalkPrecision;
  random_walk_chance_ = constants::kBasicStrategyRandomWalkChance;
  random_walk_distance_ = constants::kBasicStrategyRandomWalkDistance;
  state_ = State::kStay;
}

void BasicStrategy::DecreaseIntervals(double time) {
  attack_interval_ = std::max(attack_interval_ - time, 0.0);
  walk_interval_ = std::max(walk_interval_ - time, 0.0);
}

void BasicStrategy::Update(double time) {
  DecreaseIntervals(time);
  UpdateConditions();
  if (IsActionFinished()) {
    SelectNewState();
  }
  PerformAction();
}

void BasicStrategy::UpdateStay() {
  bool can_walk = true;
  if (HasCondition(Condition::kSeeEnemy) &&
      HasCondition(Condition::kCanAttack)) {
    if (attack_interval_ == 0) {
      state_ = State::kAttack;
      attack_interval_ = constants::kBasicStrategyAttackTimeCount;
    }
    can_walk = false;
  }
  if (can_walk) {
    state_ = State::kWalk;
    walk_interval_ = constants::kBasicStrategyWalkTimeCount;
    walk_target_ = ChooseRandomWalkPosition();
  }
}

void BasicStrategy::UpdateWalk() {
  if (HasCondition(Condition::kSeeEnemy)) {
    if (HasCondition(Condition::kCanAttack)) {
      if (attack_interval_ == 0) {
        state_ = State::kAttack;
        attack_interval_ = constants::kBasicStrategyAttackTimeCount;
      } else {
        state_ = State::kStay;
      }
      return;
    }
    walk_target_ = attack_target_->GetPosition();
    walk_interval_ = constants::kBasicStrategyWalkTimeCount;
    return;
  }
  state_ = State::kStay;
}
void BasicStrategy::UpdateAttack() {
  state_ = State::kWalk;
  walk_interval_ = constants::kBasicStrategyWalkTimeCount;
}

void BasicStrategy::SelectNewState() {
  switch (state_) {
    case State::kStay:
      UpdateStay();
      break;
    case State::kWalk:
      UpdateWalk();
      break;
    case State::kAttack:
      UpdateAttack();
      break;
    default:
      assert(false);
      break;
  }
}

QPointF BasicStrategy::ChooseRandomWalkPosition() const {
  return {
      GetMobState().GetPos().x() +
          utils::GetRandomDouble(-constants::kBasicStrategyRandomWalkDistance,
                                 constants::kBasicStrategyRandomWalkDistance),
      GetMobState().GetPos().y()};
}

void BasicStrategy::UpdateConditions() {
  ClearConditions();
  std::shared_ptr<MovingObject> target = EnemySpotted();
  if (target) {
    AddCondition(Condition::kSeeEnemy);
    if (MovingObject::IsObjectCollision(
            GetMobState().GetPos(), GetMobState().GetSize(),
            target->GetPosition(), target->GetSize())) {
      AddCondition(Condition::kCanAttack);
    }
    walk_target_ = target->GetPosition();
  }
  if (attack_target_ && !target) {
    walk_target_ = ChooseRandomWalkPosition();
  }
  attack_target_ = target;
}

bool BasicStrategy::AlmostNearX(QPointF lhs, QPointF rhs) {
  return std::abs(lhs.x() - rhs.x()) <= constants::kBasicStrategyWalkPrecision;
}

bool BasicStrategy::IsActionFinished() {
  switch (state_) {
    case State::kStay:
      if (HasCondition(Condition::kSeeEnemy)) {
        return true;
      }
      static std::uniform_real_distribution<double> distrib(0.0, 1.0);
      return distrib(utils::random) < constants::kBasicStrategyRandomWalkChance;
    case State::kWalk:
      if (HasCondition(Condition::kCanAttack) && attack_interval_ == 0) {
        return true;
      }
      if (walk_interval_ == 0) {
        return true;
      }
      return (AlmostNearX(GetMobState().GetPos(), walk_target_));
    case State::kAttack:
      if (HasCondition(Condition::kSeeEnemy) &&
          HasCondition(Condition::kCanAttack)) {
        if (attack_interval_ == 0) {
          return false;
        }
      }
      return true;
    default:
      return false;
  }
}

void BasicStrategy::PerformAction() {
  switch (state_) {
    case State::kStay:
      DoStay();
      break;
    case State::kWalk:
      DoWalk();
      break;
    case State::kAttack:
      DoAttack();
      break;
    default:
      break;
  }
}

void BasicStrategy::DoStay() { keys_.clear(); }

bool BasicStrategy::IsNearPit(QPointF src, utils::Direction side) const {
  int direction = (side == utils::Direction::kLeft) ? -1 : 1;
  int x = (side == utils::Direction::kLeft)
              ? std::floor(src.x() + GetMobState().GetSize().x() / 2)
              : std::floor(src.x() + GetMobState().GetSize().x() -
                           GetMobState().GetSize().x() / 2);
  int y = std::floor(src.y() + GetMobState().GetSize().y() + constants::kEps);
  int length = GetMobState().GetJump().x();
  int height = GetMobState().GetJump().y();
  for (int j = 0; j < length; j++) {
    bool is_pit_near = true;
    int position = height + 1;
    for (int i = 0; i <= height; i++) {
      if (Model::GetInstance()
              ->GetMap()
              ->GetBlock(QPoint(x + direction * j, y + i))
              .GetType() != Block::Type::kAir) {
        is_pit_near = false;
        position = i;
        break;
      }
    }
    if (j == 0 && position <= 1) {
      return false;
    }
    if (is_pit_near) {
      return true;
    }
  }
  return false;
}

void BasicStrategy::DoWalkActions() {
  // Do nothing
}

void BasicStrategy::DoWalk() {
  keys_.clear();
  QPointF src = GetMobState().GetPos();
  QPointF dst = walk_target_;
  if (AlmostNearX(src, dst)) {
    return;
  }
  std::shared_ptr<AbstractMap> map = Model::GetInstance()->GetMap();
  bool succesful_jump_up = true;
  for (int i = GetMobState().GetPos().y();
       i >= GetMobState().GetPos().y() - GetMobState().GetSize().y(); i--) {
    if (map->GetBlock(QPoint(static_cast<int32_t>(
                                 std::floor(GetMobState().GetPos().x())),
                             i))
            .GetType() != Block::Type::kAir) {
      succesful_jump_up = false;
      break;
    }
  }
  if (src.x() > dst.x()) {
    keys_.insert(ControllerTypes::Key::kLeft);
    if (GetMobState().IsPushesLeft()) {
      if (succesful_jump_up) {
        keys_.insert(ControllerTypes::Key::kJump);
      }
    }
    if (src.y() >= dst.y() - GetMobState().GetJump().y()) {
      if (IsNearPit(src, utils::Direction::kLeft)) {
        keys_.insert(ControllerTypes::Key::kJump);
      }
    }
  } else {
    keys_.insert(ControllerTypes::Key::kRight);
    if (GetMobState().IsPushesRight()) {
      if (succesful_jump_up) {
        keys_.insert(ControllerTypes::Key::kJump);
      }
    }
    if (src.y() >= dst.y() - GetMobState().GetJump().y()) {
      if (IsNearPit(src, utils::Direction::kRight)) {
        keys_.insert(ControllerTypes::Key::kJump);
      }
    }
  }
  DoWalkActions();
}

void BasicStrategy::DoAttack() {
  Damage damage(GetMobState().GetPos(), Damage::Type::kMob,
                GetMobState().GetDamage(),
                GetMobState().GetDamageAcceleration());
  attack_target_->DealDamage(damage);
}

static double EuclidianDistance(QPointF lhs, QPointF rhs) {
  return std::hypot(lhs.x() - rhs.x(), lhs.y() - rhs.y());
}

std::shared_ptr<MovingObject> BasicStrategy::EnemySpotted() {
  if (EuclidianDistance(Model::GetInstance()->GetPlayer()->GetPosition(),
                        GetMobState().GetPos()) <=
      constants::kBasicStrategyVisionRadius) {
    auto target = Model::GetInstance()->GetPlayer();
    return target;
  }
  return nullptr;
}

ZombieSummonerStrategy::ZombieSummonerStrategy() : BasicStrategy() {
  vision_radius_ = constants::kZombieSummonerStrategyVisionRadius;
  walk_time_count_ = constants::kZombieSummonerStrategyWalkTime;
  attack_time_count_ = constants::kZombieSummonerStrategyAttackTime;
  walk_precision_ = constants::kZombieSummonerStrategyWalkPrecision;
  random_walk_chance_ = constants::kZombieSummonerStrategyRandomWalkChance;
  random_walk_distance_ = constants::kZombieSummonerStrategyRandomWalkDistance;
}

void ZombieSummonerStrategy::DecreaseIntervals(double time) {
  BasicStrategy::DecreaseIntervals(time);
  summon_interval = std::max(summon_interval - time, 0.0);
}

void ZombieSummonerStrategy::SummonZombie() {
  for (int op = constants::kZombieSummonerStrategySummonAttempts; op > 0;
       op--) {
    std::uniform_real_distribution<double> distrib_x(
        GetMobState().GetPos().x() -
            constants::kZombieSummonerStrategySummonDistance,
        GetMobState().GetPos().x() +
            constants::kZombieSummonerStrategySummonDistance);
    std::uniform_real_distribution<double> distrib_y(
        GetMobState().GetPos().y() -
            constants::kZombieSummonerStrategySummonDistance,
        GetMobState().GetPos().y() +
            constants::kZombieSummonerStrategySummonDistance);
    double pos_x = distrib_x(utils::random);
    double pos_y = distrib_y(utils::random);
    if (Model::GetInstance()->CanSpawnMobAt(QPointF(pos_x, pos_y),
                                            GetMobState().GetSize())) {
      Model::GetInstance()->AddMob(
          std::make_shared<Mob>(QPointF(pos_x, pos_y), Mob::Type::kZombie));
      return;
    }
  }
}

void ZombieSummonerStrategy::DoWalkActions() {
  if (GetMobState().GetDamageTime() > constants::kEps && summon_interval == 0) {
    summon_interval = constants::kZombieSummonerStrategySummonTime;
    static std::uniform_real_distribution<double> distrib(0.0, 1.0);
    if (distrib(utils::random) <
        constants::kZombieSummonerStrategySummonChance) {
      SummonZombie();
    }
  }
}

MagicStrategy::MagicStrategy() {}

void MagicStrategy::DoAttack() {
  Damage damage(GetMobState().GetPos(), Damage::Type::kMob,
                GetMobState().GetDamage(),
                GetMobState().GetDamageAcceleration());
  attack_target_->DealDamage(damage);
  static std::uniform_real_distribution<double> check_distrib(0.0, 1.0);
  if (check_distrib(utils::random) > constants::kMagicStrategyEffectChance) {
    return;
  }
  Effect effect(Effect::Type::kPoison);
  static std::uniform_int_distribution<int> id_distrib(0, 3);
  static std::uniform_real_distribution<double> strength_distrib(1.0, 1.25);
  effect.SetStrength(strength_distrib(utils::random));
  effect.SetTime(constants::kMagicStrategyEffectDuration);
  int effect_id = id_distrib(utils::random);
  switch (effect_id) {
    case 0:
      effect.SetType(Effect::Type::kPoison);
      break;
    case 1:
      effect.SetType(Effect::Type::kSlowness);
      break;
    case 2:
      effect.SetType(Effect::Type::kWeakness);
      break;
    case 3:
      effect.SetType(Effect::Type::kHeaviness);
      break;
    default:
      break;
  }
  qDebug() << "Effect:" << effect_id << " " << effect.GetStrength() << " "
           << effect.GetTime();
  attack_target_->AddEffect(effect);
}
