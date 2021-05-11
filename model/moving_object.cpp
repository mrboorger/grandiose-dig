#include "model/moving_object.h"

#include <algorithm>
#include <cmath>

#include "model/model.h"
#include "utils.h"

MovingObject::MovingObject(QPointF pos, QPointF size)
    : pos_(pos), size_(size) {}

bool MovingObject::IsObjectCollision(QPointF lhs_pos, QPointF lhs_size,
                                     QPointF rhs_pos, QPointF rhs_size) {
  QRectF lhs(lhs_pos, QSizeF(lhs_size.x(), lhs_size.y()));
  QRectF rhs(rhs_pos, QSizeF(rhs_size.x(), rhs_size.y()));
  return lhs.intersects(rhs);
}

void MovingObject::Move(
    const std::unordered_set<ControllerTypes::Key>& pressed_keys, double time) {
  UpdateState(pressed_keys, time);
}

void MovingObject::UpdateStay(
    const std::unordered_set<ControllerTypes::Key>& pressed_keys, double time) {
  Q_UNUSED(time);
  if (!pushes_ground_) {
    state_ = State::kJump;
    move_vector_.SetMomentum(move_vector_.GetSpeed());
    move_vector_.ResetSpeed();
    return;
  }
  move_vector_.ResetSpeed();
  if (pressed_keys.count(ControllerTypes::Key::kLeft) !=
      pressed_keys.count(ControllerTypes::Key::kRight)) {
    state_ = State::kWalk;
  } else if (pressed_keys.count(ControllerTypes::Key::kJump)) {
    state_ = State::kJump;
    move_vector_.SetMomentum(0, jump_speed_);
  }
}

// TODO(Wind-Eagle): Do not reset acceleration when jumping off the block
void MovingObject::UpdateWalk(
    const std::unordered_set<ControllerTypes::Key>& pressed_keys, double time) {
  if (pressed_keys.count(ControllerTypes::Key::kLeft) ==
      pressed_keys.count(ControllerTypes::Key::kRight)) {
    state_ = State::kStay;
    move_vector_.SetSpeedX(0);
  } else if (pressed_keys.count(ControllerTypes::Key::kRight)) {
    if (pushes_right_) {
      move_vector_.SetSpeedX(0);
    } else {
      move_vector_.TranslateSpeedXIfNearerToBounds(walk_acceleration_ * time, 0,
                                                   walk_max_speed_);
    }
  } else if (pressed_keys.count(ControllerTypes::Key::kLeft)) {
    if (pushes_left_) {
      move_vector_.SetSpeedX(0);
    } else {
      move_vector_.TranslateSpeedXIfNearerToBounds(-walk_acceleration_ * time,
                                                   -walk_max_speed_, 0);
    }
  }
  if (pushes_ground_) {
    if (pressed_keys.count(ControllerTypes::Key::kJump)) {
      move_vector_.SetSpeedY(jump_speed_);
    }
  } else {
    state_ = State::kJump;
    move_vector_.SetMomentum(move_vector_.GetSpeed());
    move_vector_.ResetSpeed();
  }
}

void MovingObject::UpdateJump(
    const std::unordered_set<ControllerTypes::Key>& pressed_keys, double time) {
  move_vector_.TranslateSpeedWithLimits(0, gravity_speed_ * time);
  if (pressed_keys.count(ControllerTypes::Key::kLeft) ==
      pressed_keys.count(ControllerTypes::Key::kRight)) {
    // Do nothing
  } else if (pressed_keys.count(ControllerTypes::Key::kRight)) {
    if (pushes_right_) {
      move_vector_.SetSpeedX(0);
    } else {
      move_vector_.TranslateSpeedXIfNearerToBounds(
          walk_air_acceleration_ * time,
          -walk_max_air_acceleration_ + move_vector_.GetMomentumX(),
          walk_max_air_acceleration_ - move_vector_.GetMomentumX());
    }
  } else if (pressed_keys.count(ControllerTypes::Key::kLeft)) {
    if (pushes_left_) {
      move_vector_.SetSpeedX(0);
    } else {
      move_vector_.TranslateSpeedXIfNearerToBounds(
          -walk_air_acceleration_ * time,
          -walk_max_air_acceleration_ - move_vector_.GetMomentumX(),
          walk_max_air_acceleration_ + move_vector_.GetMomentumX());
    }
  }
  if (pushes_ground_) {
    if (pressed_keys.count(ControllerTypes::Key::kLeft) ==
        pressed_keys.count(ControllerTypes::Key::kRight)) {
      state_ = State::kStay;
      move_vector_.ResetSpeed();
    } else {
      state_ = State::kWalk;
      move_vector_.SetSpeedY(0);
    }
  } else {
    move_vector_.SetSpeedX(move_vector_.GetSpeedX() *
                           (1.0 - constants::kAirResistance));
    move_vector_.SetMomentumX(move_vector_.GetMomentumX() *
                              (1.0 - constants::kAirResistance));
  }
}

void MovingObject::UpdateState(
    const std::unordered_set<ControllerTypes::Key>& pressed_keys, double time) {
  QPointF old_position = pos_;
  State old_state = state_;
  damage_time_ = std::max(damage_time_ - time, 0.0);
  switch (state_) {
    case State::kStay:
      UpdateStay(pressed_keys, time);
      break;
    case State::kWalk:
      UpdateWalk(pressed_keys, time);
      break;
    case State::kJump:
      UpdateJump(pressed_keys, time);
      break;
  }
  if (old_state == state_) {
    state_time_ += time;
  } else {
    state_time_ = 0;
  }
  if (state_ != State::kJump) {
    move_vector_.ResetMomentum();
  }
  if (pressed_keys.count(ControllerTypes::Key::kLeft) ==
      pressed_keys.count(ControllerTypes::Key::kRight)) {
    // Do nothing
  } else if (pressed_keys.count(ControllerTypes::Key::kRight)) {
    SetDirection(utils::Direction::kRight);
  } else if (pressed_keys.count(ControllerTypes::Key::kLeft)) {
    SetDirection(utils::Direction::kLeft);
  }
  DecEffects(time);
  MakeMovement(old_position, time);
}

bool MovingObject::FindCollisionGround(
    QPointF old_position, double* ground_y,
    const std::shared_ptr<AbstractMap>& map) const {
  QPointF old_bottom_left = old_position + QPointF(0, size_.y());
  QPointF new_bottom_left = pos_ + QPointF(0, size_.y());
  int end_y = std::floor(new_bottom_left.y());
  int begin_y =
      std::min(static_cast<int>(std::floor(old_bottom_left.y()) + 1), end_y);
  int dist = std::max(std::abs(end_y - begin_y), 1);
  for (int y = begin_y; y <= end_y; y++) {
    double bottom_left_x =
        utils::DivideSegment(old_bottom_left, new_bottom_left,
                             static_cast<double>(std::abs(end_y - y)) / dist)
            .x();
    double bottom_right_x = bottom_left_x + size_.x() - constants::kEps;
    for (double x = bottom_left_x + constants::kEps;; x += 1) {
      QPoint block_pos{static_cast<int>(std::floor(x)),
                       static_cast<int>(std::floor(y))};
      if (map->GetBlock(block_pos).GetType() != Block::Type::kAir) {
        *ground_y = block_pos.y() - size_.y();
        return true;
      }
      if (x >= std::floor(bottom_right_x)) {
        break;
      }
    }
  }
  return false;
}

bool MovingObject::FindCollisionCeiling(
    QPointF old_position, double* ceiling_y,
    const std::shared_ptr<AbstractMap>& map) const {
  QPointF old_top_left = old_position;
  QPointF new_top_left = pos_;
  int end_y = std::floor(new_top_left.y());
  int begin_y =
      std::max(static_cast<int>(std::floor(old_top_left.y()) - 1), end_y);
  int dist = std::max(std::abs(end_y - begin_y), 1);
  for (int y = begin_y; y >= end_y; y--) {
    double top_left_x =
        utils::DivideSegment(old_top_left, new_top_left,
                             static_cast<double>(std::abs(end_y - y)) / dist)
            .x();
    double top_right_x = top_left_x + size_.x() - constants::kEps;
    for (double x = top_left_x + constants::kEps;; x += 1) {
      QPoint block_pos{static_cast<int>(std::floor(x)),
                       static_cast<int>(std::floor(y))};
      if (map->GetBlock(block_pos).GetType() != Block::Type::kAir) {
        *ceiling_y = block_pos.y() + 1;
        return true;
      }
      if (x >= std::floor(top_right_x)) {
        break;
      }
    }
  }
  return false;
}

bool MovingObject::FindCollisionLeft(
    QPointF old_position, double* left_wall_x,
    const std::shared_ptr<AbstractMap>& map) const {
  QPointF old_bottom_left =
      old_position + QPointF(0, size_.y() - constants::kEps);
  QPointF new_bottom_left = pos_ + QPointF(0, size_.y() - constants::kEps);
  int end_x = std::floor(new_bottom_left.x());
  int begin_x =
      std::max(static_cast<int>(std::floor(old_bottom_left.x()) - 1), end_x);
  int dist = std::max(std::abs(end_x - begin_x), 1);
  for (int x = begin_x; x >= end_x; x--) {
    double bottom_left_y =
        utils::DivideSegment(old_bottom_left, new_bottom_left,
                             static_cast<double>(std::abs(end_x - x)) / dist)
            .y();
    double top_left_y = bottom_left_y - size_.y() + constants::kEps;
    for (double y = bottom_left_y;; y -= 1) {
      QPoint block_pos{static_cast<int>(std::floor(x)),
                       static_cast<int>(std::floor(y))};
      if (map->GetBlock(block_pos).GetType() != Block::Type::kAir) {
        *left_wall_x = block_pos.x() + 1;
        return true;
      }
      if (block_pos.y() <= top_left_y) {
        break;
      }
    }
  }
  return false;
}

bool MovingObject::FindCollisionRight(
    QPointF old_position, double* right_wall_x,
    const std::shared_ptr<AbstractMap>& map) const {
  QPointF old_bottom_right =
      old_position +
      QPointF(size_.x() - constants::kEps, size_.y() - constants::kEps);
  QPointF new_bottom_right =
      pos_ + QPointF(size_.x() - constants::kEps, size_.y() - constants::kEps);
  int end_x = std::floor(new_bottom_right.x());
  int begin_x =
      std::min(static_cast<int>(std::floor(old_bottom_right.x()) + 1), end_x);
  int dist = std::max(std::abs(end_x - begin_x), 1);
  for (int x = begin_x; x <= end_x; x++) {
    double bottom_right_y =
        utils::DivideSegment(old_bottom_right, new_bottom_right,
                             static_cast<double>(std::abs(end_x - x)) / dist)
            .y();
    double top_right_y = bottom_right_y - size_.y() + constants::kEps;
    for (double y = bottom_right_y;; y -= 1) {
      QPoint block_pos{static_cast<int>(std::floor(x)),
                       static_cast<int>(std::floor(y))};
      if (map->GetBlock(block_pos).GetType() != Block::Type::kAir) {
        *right_wall_x = block_pos.x() - size_.x();
        return true;
      }
      if (block_pos.y() <= top_right_y) {
        break;
      }
    }
  }
  return false;
}

void MovingObject::MakeMovement(QPointF old_position, double time) {
  pos_ += move_vector_.GetSpeed() * time;
  pos_ += move_vector_.GetMomentum() * time;
  CheckCollisions(old_position);
}

void MovingObject::CheckCollisions(QPointF old_position) {
  auto map = Model::GetInstance()->GetMap();
  double ground_y = 0, ceiling_y = 0;
  double right_wall_x = 0, left_wall_x = 0;
  if (move_vector_.GetSpeedX() + move_vector_.GetMomentumX() <=
          constants::kEps &&
      FindCollisionLeft(old_position, &left_wall_x, map)) {
    if (old_position.x() + constants::kEps >= left_wall_x) {
      pushes_left_ = true;
      pos_.setX(left_wall_x);
    }
    move_vector_.SetSpeedX(0);
    move_vector_.SetMomentumX(0);
  } else {
    pushes_left_ = false;
  }
  if (move_vector_.GetSpeedX() + move_vector_.GetMomentumX() >=
          -constants::kEps &&
      FindCollisionRight(old_position, &right_wall_x, map)) {
    if (old_position.x() - constants::kEps <= right_wall_x) {
      pushes_right_ = true;
      pos_.setX(right_wall_x);
    }
    move_vector_.SetSpeedX(0);
    move_vector_.SetMomentumX(0);
  } else {
    pushes_right_ = false;
  }
  if (move_vector_.GetSpeedY() + move_vector_.GetMomentumY() <=
          constants::kEps &&
      FindCollisionCeiling(old_position, &ceiling_y, map)) {
    pushes_ceil_ = true;
    pos_.setY(ceiling_y);
    move_vector_.SetSpeedY(0);
    move_vector_.SetMomentumY(0);
  } else {
    pushes_ceil_ = false;
  }
  if (move_vector_.GetSpeedY() + move_vector_.GetMomentumY() >=
          -constants::kEps &&
      FindCollisionGround(old_position, &ground_y, map)) {
    pushes_ground_ = true;
    CheckFallDamage();
    pos_.setY(ground_y);
    move_vector_.SetSpeedY(0);
    move_vector_.SetMomentumY(0);
  } else {
    pushes_ground_ = false;
  }
}

void MovingObject::CheckFallDamage() {
  double fall_damage_speed =
      move_vector_.GetSpeedY() + move_vector_.GetMomentumY();
  if (fall_damage_speed > constants::kFallDamageMin) {
    Damage damage(Damage::Type::kFall,
                  std::ceil((fall_damage_speed - constants::kFallDamageMin) /
                            constants::kFallDamagePoint),
                  QPointF(0, 0));
    DealDamage(damage);
  }
}

void MovingObject::DealDamage(const Damage& damage) {
  if (IsDead()) {
    return;
  }
  if (damage.GetType() != Damage::Type::kMagic && RecentlyDamaged()) {
    return;
  }
  damage_time_ = constants::kDamageCooldown;
  health_ -= damage.GetAmount();
  if (IsDead()) {
    emit Model::GetInstance()->BecameDead(type_);
  } else {
    emit Model::GetInstance()->DamageDealt(type_);
  }
  if (damage.GetType() == Damage::Type::kMob ||
      damage.GetType() == Damage::Type::kPlayer) {
    QPointF source = damage.GetSource();
    QPointF damage_push = damage.GetDamageAcceleration();
    if (source.x() > pos_.x()) {
      damage_push.setX(-damage_push.x());
    }
    if (!pushes_ground_) {
      damage_push.setY(0);
    }
    // TranslateSpeed(damage_push) causes a non-realistic behaviour:
    // in some cases there is too high speed of a damage push
    move_vector_.SetSpeedX(damage_push.x());
    move_vector_.TranslateSpeed({0, damage_push.y()});
    MakeMovement(
        pos_,
        constants::kEps);  // recalculates many parameters, excluding position
  }
  qDebug() << "Damage: " << health_;
}

bool MovingObject::IsDead() const { return health_ <= 0; }

void MovingObject::AddEffect(Effect effect) {
  Effect::Type type = effect.GetType();
  DeleteEffect(type);
  switch (type) {
    case Effect::Type::kSpeed:
      DeleteEffect(Effect::Type::kSlowness);
      break;
    case Effect::Type::kSlowness:
      DeleteEffect(Effect::Type::kSpeed);
      break;
    case Effect::Type::kStrength:
      DeleteEffect(Effect::Type::kWeakness);
      break;
    case Effect::Type::kWeakness:
      DeleteEffect(Effect::Type::kStrength);
      break;
    case Effect::Type::kRegeneration:
      DeleteEffect(Effect::Type::kPoison);
      break;
    case Effect::Type::kPoison:
      DeleteEffect(Effect::Type::kRegeneration);
      break;
    case Effect::Type::kLightness:
      DeleteEffect(Effect::Type::kHeaviness);
      break;
    case Effect::Type::kHeaviness:
      DeleteEffect(Effect::Type::kLightness);
      break;
    default:
      break;
  }
  effects_.push_back(effect);
  ApplyEffect(effect);
}

void MovingObject::DeleteEffect(Effect::Type type) {
  auto element = std::find_if(
      effects_.begin(), effects_.end(),
      [&type](const Effect& effect) { return effect.GetType() == type; });
  if (element == effects_.end()) {
    return;
  }
  auto effect = *element;
  UnapplyEffect(effect);
  effects_.erase(element);
}

void MovingObject::DecEffects(double time) {
  for (auto& effect : effects_) {
    double prev = effect.GetTime();
    effect.DecTime(time);
    if (std::round(effect.GetTime() / constants::kEffectInterval) !=
        std::round(prev / constants::kEffectInterval)) {
      effect.ActivateEffect();
    }
  }
  for (auto& effect : effects_) {
    if (effect.GetTime() < constants::kEps) {
      UnapplyEffect(effect);
    }
  }
  effects_.erase(std::remove_if(effects_.begin(), effects_.end(),
                                [](const Effect& effect) {
                                  return effect.GetTime() < constants::kEps;
                                }),
                 effects_.end());
  CheckSingularEffects();
}

namespace {

static double GetMultiplyCoef(double coef, EffectProcessType type) {
  return type == EffectProcessType::kForward ? coef : 1.0 / coef;
}

}  // namespace

void MovingObject::ProcessEffect(Effect effect, EffectProcessType type) {
  Effect::Type effect_type = effect.GetType();
  switch (effect_type) {
    case Effect::Type::kSpeed:
      walk_acceleration_ *= GetMultiplyCoef(
          constants::kSpeedEffectMultiplier * effect.GetStrength(), type);
      walk_max_speed_ *= GetMultiplyCoef(
          constants::kSpeedEffectMultiplier * effect.GetStrength(), type);
      walk_air_acceleration_ *= GetMultiplyCoef(
          constants::kSpeedEffectMultiplier * effect.GetStrength(), type);
      walk_max_air_acceleration_ *= GetMultiplyCoef(
          constants::kSpeedEffectMultiplier * effect.GetStrength(), type);
      break;
    case Effect::Type::kSlowness:
      walk_acceleration_ *= GetMultiplyCoef(
          constants::kSpeedEffectMultiplier * effect.GetStrength(), type);
      walk_max_speed_ *= GetMultiplyCoef(
          constants::kSpeedEffectMultiplier * effect.GetStrength(), type);
      walk_air_acceleration_ *= GetMultiplyCoef(
          constants::kSpeedEffectMultiplier * effect.GetStrength(), type);
      walk_max_air_acceleration_ *= GetMultiplyCoef(
          constants::kSpeedEffectMultiplier * effect.GetStrength(), type);
      break;
    case Effect::Type::kStrength:
      damage_ *= GetMultiplyCoef(
          constants::kStrengthEffectMultiplier * effect.GetStrength(), type);
      break;
    case Effect::Type::kWeakness:
      damage_ *= GetMultiplyCoef(
          constants::kWeaknessEffectMultiplier * effect.GetStrength(), type);
      break;
    case Effect::Type::kLightness:
      gravity_speed_ *= GetMultiplyCoef(
          constants::kLightnessEffectMultiplier * effect.GetStrength(), type);
      break;
    case Effect::Type::kHeaviness:
      gravity_speed_ *= GetMultiplyCoef(
          constants::kHeavinessEffectMultiplier * effect.GetStrength(), type);
      break;
    default:
      break;
  }
}

void MovingObject::ApplySingularEffect(Effect effect) {
  Effect::Type type = effect.GetType();
  switch (type) {
    case Effect::Type::kRegeneration:  // TODO(Wind-Eagle): health can become
                                       // bigger, than initial/maximal value
      health_ +=
          constants::kRegenerationEffectMultiplier * effect.GetStrength();
      break;
    case Effect::Type::kPoison:
      DealDamage(Damage(Damage::Type::kMagic,
                        static_cast<int>(constants::kPoisonEffectMultiplier *
                                         effect.GetStrength()),
                        QPointF(0, 0)));
      break;
    default:
      break;
  }
}

void MovingObject::CheckSingularEffects() {
  for (auto& effect : effects_) {
    if (effect.IsActive()) {
      ApplySingularEffect(effect);
      effect.DeactivateEffect();
    }
  }
}
