#include "model/mob.h"

Mob::Mob(QPointF pos, QPointF size)
    : MovingObject(pos, size), mob_state_(GetPosition(), GetSize()) {
  type_ = Type::kMob;
  strategy_ = new BasicStrategy();
  SetWalkAcceleration(constants::kMobWalkAcceleration);
  SetWalkMaxSpeed(constants::kMobWalkMaxSpeed);
  SetWalkMaxAirAcceleration(constants::kMobWalkMaxAirAcceleration);
  SetJumpSpeed(constants::kMobJumpSpeed);
}

void Mob::MoveMob() {
  strategy_->SetMobState(MobState{GetPosition(), GetSize(), IsOnGround(),
                                  IsOnCeil(), IsPushesLeft(), IsPushesRight()});
  strategy_->Update();
  UpdateState(strategy_->GetKeys());
}
