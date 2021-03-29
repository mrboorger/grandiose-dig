#include "model/mob.h"

Mob::Mob(QPointF pos, QPointF size)
    : MovingObject(pos, size), mob_state_(GetPosition(), GetSize()) {
  type_ = Type::kMob;
  strategy_ = new BasicStrategy();
  SetWalkAcceleration(constants::kPlayerWalkAcceleration / 3);
  SetWalkMaxSpeed(constants::kPlayerWalkMaxSpeed / 2);
  SetWalkMaxAirAcceleration(constants::kPlayerWalkMaxAirAcceleration / 2);
  SetJumpSpeed(constants::kPlayerJumpSpeed / 1.5);
}

void Mob::MoveMob() {
  strategy_->SetMobState({GetPosition(), GetSize(), IsOnGround(), IsOnCeil(),
                          IsPushesLeft(), IsPushesRight()});
  strategy_->Update();
  UpdateState(strategy_->GetKeys());
}
