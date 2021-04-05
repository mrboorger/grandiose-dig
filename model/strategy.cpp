#include "model/strategy.h"

#include <QDebug>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <memory>
#include <random>

#include "model/model.h"

BasicStrategy::BasicStrategy() { state_ = State::kStay; }

void BasicStrategy::DecreaseIntervals() {
  attack_interval_ = std::max(attack_interval_ - 1, 0);
  walk_interval_ = std::max(walk_interval_ - 1, 0);
}

void BasicStrategy::Update() {
  DecreaseIntervals();
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
      attack_interval_ = constants::kBasicStrategyAttackTicksCount;
    }
    can_walk = false;
  }
  if (can_walk) {
    state_ = State::kWalk;
    walk_interval_ = constants::kBasicStrategyWalkTicksCount;
    walk_target_ = ChooseRandomWalkPosition();
  }
}

void BasicStrategy::UpdateWalk() {
  if (HasCondition(Condition::kSeeEnemy)) {
    if (HasCondition(Condition::kCanAttack)) {
      if (attack_interval_ == 0) {
        state_ = State::kAttack;
        attack_interval_ = constants::kBasicStrategyAttackTicksCount;
      } else {
        state_ = State::kStay;
      }
      return;
    }
    walk_target_ = attack_target_->GetPosition();
    walk_interval_ = constants::kBasicStrategyWalkTicksCount;
    return;
  }
  state_ = State::kStay;
}
void BasicStrategy::UpdateAttack() {
  state_ = State::kWalk;
  walk_interval_ = constants::kBasicStrategyWalkTicksCount;
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
  // TODO(Wind-Eagle): This is temporary code.
  static std::mt19937 rnd(time(nullptr));
  return {GetMobState().GetPos().x() + static_cast<double>(rnd() % 21) - 10,
          GetMobState().GetPos().y()};
}

void BasicStrategy::UpdateConditions() {
  ClearConditions();
  std::shared_ptr<const MovingObject> target = EnemySpotted();
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

static bool AlmostNearX(QPointF lhs, QPointF rhs) {
  return std::abs(lhs.x() - rhs.x()) <= constants::kBasicStrategyWalkPrecision;
}

bool BasicStrategy::IsActionFinished() {
  static std::mt19937 rnd(time(nullptr));
  switch (state_) {
    case State::kStay:
      if (HasCondition(Condition::kSeeEnemy)) {
        return true;
      }
      static std::uniform_real_distribution<double> distrib(0.0, 1.0);
      return distrib(rnd) < constants::kBasicStrategyRandomWalkChance;
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
  } else {
    keys_.insert(ControllerTypes::Key::kRight);
    if (GetMobState().IsPushesRight()) {
      if (succesful_jump_up) {
        keys_.insert(ControllerTypes::Key::kJump);
      }
    }
  }
}

void BasicStrategy::DoAttack() {
  // TODO(Wind-Eagle): This is temporary code
  qDebug() << "Attack";
}

static double EuclidianDistance(QPointF lhs, QPointF rhs) {
  return std::hypot(lhs.x() - rhs.x(), lhs.y() - rhs.y());
}

std::shared_ptr<const MovingObject> BasicStrategy::EnemySpotted() {
  if (EuclidianDistance(Model::GetInstance()->GetPlayer()->GetPosition(),
                        GetMobState().GetPos()) <=
      constants::kBasicStrategyVisionRadius) {
    auto target = Model::GetInstance()->GetPlayer();
    return target;
  }
  return nullptr;
}
