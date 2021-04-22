#include "model/mob.h"

Mob::Mob(QPointF pos, QPointF size)
    : MovingObject(pos, size),
      mob_state_(GetPosition(), GetSize()),
      type_(Type::kMob),
      strategy_(std::make_shared<BasicStrategy>()) {
  SetWalkAcceleration(constants::kMobWalkAcceleration);
  SetWalkMaxSpeed(constants::kMobWalkMaxSpeed);
  SetWalkMaxAirAcceleration(constants::kMobWalkMaxAirAcceleration);
  SetJumpSpeed(constants::kMobJumpSpeed);
  SetDamageAcceleration(constants::kMobDamageAcceleration);
  SetHealth(constants::kMobHealth);
  SetDamage(constants::kMobDamage);
  SetType(MovingObject::Type::kMob);
}

void Mob::CalculateMobMovement(double time) {
  strategy_->SetMobState(MobState(GetPosition(), GetSize(), GetDamage(),
                                  IsOnGround(), IsOnCeil(), IsPushesLeft(),
                                  IsPushesRight()));
  strategy_->Update(time);
}

void Mob::MoveMob(double time) {
  UpdateState(strategy_->GetKeys(),
              time);  // maybe better to call MovingObject::Move
}
