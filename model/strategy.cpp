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
  MakeAction();
}

void BasicStrategy::SelectNewState() {
  switch (state_) {
    case State::kStay:
      if (std::find(conditions_.begin(), conditions_.end(),
                    Condition::kSeeEnemy) != conditions_.end()) {
        if (std::find(conditions_.begin(), conditions_.end(),
                      Condition::kCanAttack) != conditions_.end()) {
          if (attack_interval_ == 0) {
            state_ = State::kAttack;
            attack_interval_ = constants::kBasicStrategyAttackTicksCount;
          }
        } else {
          state_ = State::kWalk;
          walk_interval_ = constants::kBasicStrategyWalkTicksCount;
          walk_target_ = ChooseRandomWalkPosition();
        }
        break;
      }
      state_ = State::kWalk;
      walk_interval_ = constants::kBasicStrategyWalkTicksCount;
      walk_target_ = ChooseRandomWalkPosition();
      break;
    case State::kWalk:
      if (std::find(conditions_.begin(), conditions_.end(),
                    Condition::kSeeEnemy) != conditions_.end()) {
        if (std::find(conditions_.begin(), conditions_.end(),
                      Condition::kCanAttack) != conditions_.end()) {
          if (attack_interval_ == 0) {
            state_ = State::kAttack;
            attack_interval_ = constants::kBasicStrategyAttackTicksCount;
          } else {
            state_ = State::kStay;
          }
          break;
        }
        walk_target_ = attack_target_->GetPosition();
        walk_interval_ = constants::kBasicStrategyWalkTicksCount;
        break;
      }
      state_ = State::kStay;
      break;
    case State::kAttack:
      state_ = State::kWalk;
      walk_interval_ = constants::kBasicStrategyWalkTicksCount;
      break;
    default:
      break;
  }
}

QPointF BasicStrategy::ChooseRandomWalkPosition() const {
  // TODO(Wind-Eagle): This is temporary code.
  static std::mt19937 rnd(time(nullptr));
  return {mob_state_.GetPos().x() + static_cast<double>(rnd() % 21) - 10,
          mob_state_.GetPos().y()};
}

void BasicStrategy::UpdateConditions() {
  conditions_.clear();
  std::shared_ptr<const MovingObject> target = EnemySpotted();
  if (target) {
    conditions_.push_back(Condition::kSeeEnemy);
    if (MovingObject::IsObjectCollision(
            mob_state_.GetPos(), mob_state_.GetSize(), target->GetPosition(),
            target->GetSize())) {
      conditions_.push_back(Condition::kCanAttack);
    }
    walk_target_ = target->GetPosition();
  }
  if (attack_target_ && (!target)) {
    walk_target_ = ChooseRandomWalkPosition();
  }
  attack_target_ = target;
}

bool AlmostNearX(QPointF lhs, QPointF rhs) {
  return std::abs(lhs.x() - rhs.x()) <= constants::kBasicStrategyWalkPrecision;
}

bool BasicStrategy::IsActionFinished() {
  static std::mt19937 rnd(time(nullptr));
  switch (state_) {
    case State::kStay:
      if (std::find(conditions_.begin(), conditions_.end(),
                    Condition::kSeeEnemy) != conditions_.end()) {
        return true;
      }
      return rnd() % constants::kBasicStrategyRandomWalkChance == 0;
    case State::kWalk:
      if (std::find(conditions_.begin(), conditions_.end(),
                    Condition::kCanAttack) != conditions_.end() &&
          attack_interval_ == 0) {
        return true;
      }
      if (walk_interval_ == 0) {
        return true;
      }
      return (AlmostNearX(mob_state_.GetPos(), walk_target_));
    case State::kAttack:
      if (std::find(conditions_.begin(), conditions_.end(),
                    Condition::kSeeEnemy) != conditions_.end() &&
          std::find(conditions_.begin(), conditions_.end(),
                    Condition::kCanAttack) != conditions_.end()) {
        if (attack_interval_ == 0) {
          return false;
        }
      }
      return true;
    default:
      return false;
  }
}

void BasicStrategy::MakeAction() {
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
  QPointF src = mob_state_.GetPos();
  QPointF dst = walk_target_;
  if (AlmostNearX(src, dst)) {
    return;
  }
  std::shared_ptr<AbstractMap> map = Model::GetInstance()->GetMap();
  bool succesful_jump_up = true;
  for (int i = mob_state_.GetPos().y();
       i >= mob_state_.GetPos().y() - mob_state_.GetSize().y(); i--) {
    if (map->GetBlock(std::floor(mob_state_.GetPos().x()), i).GetType() !=
        Block::Type::kAir) {
      succesful_jump_up = false;
      break;
    }
  }
  if (src.x() > dst.x()) {
    keys_.insert(ControllerTypes::Key::kLeft);
    if (mob_state_.IsPushesLeft()) {
      if (succesful_jump_up) {
        keys_.insert(ControllerTypes::Key::kJump);
      }
    }
  } else {
    keys_.insert(ControllerTypes::Key::kRight);
    if (mob_state_.IsPushesRight()) {
      if (succesful_jump_up) {
        keys_.insert(ControllerTypes::Key::kJump);
      }
    }
  }
}
void BasicStrategy::DoAttack() { qDebug() << "Attack"; }

double EuclidianDistance(QPointF lhs, QPointF rhs) {
  return std::sqrt((lhs.x() - rhs.x()) * (lhs.x() - rhs.x()) +
                   (lhs.y() - rhs.y()) * (lhs.y() - rhs.y()));
}

std::shared_ptr<const MovingObject> BasicStrategy::EnemySpotted() {
  if (EuclidianDistance(Model::GetInstance()->GetPlayer()->GetPosition(),
                        mob_state_.GetPos()) <=
      constants::kBasicStrategyVisionRadius) {
    auto target = std::dynamic_pointer_cast<const MovingObject>(
        Model::GetInstance()->GetPlayer());
    return target;
  }
  return nullptr;
}
