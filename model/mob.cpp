#include "model/mob.h"

namespace {

std::array<QPointF, Mob::kTypesCount> kMobSizes = {constants::kZombieSize,
                                                   constants::kZombieLordSize};

std::array<QPoint, Mob::kTypesCount> kMobJumps = {
    constants::kZombieJumpInBlocks, constants::kZombieLordJumpInBlocks};

std::array<QPointF, Mob::kTypesCount> kMobDamageAccelerations = {
    constants::kZombieDamageAcceleration,
    constants::kZombieLordDamageAcceleration};

}  // namespace

Mob::Mob(QPointF pos, Type type)
    : MovingObject(pos, kMobSizes[static_cast<int>(type)]),
      mob_state_(GetPosition(), GetSize(), kMobJumps[static_cast<int>(type)]),
      type_(type) {
  switch (type) {
    case Type::kZombie:
      SetWalkAcceleration(constants::kZombieWalkAcceleration);
      SetWalkMaxSpeed(constants::kZombieWalkMaxSpeed);
      SetWalkMaxAirAcceleration(constants::kZombieWalkMaxAirAcceleration);
      SetJumpSpeed(constants::kZombieJumpSpeed);
      SetDamageAcceleration(constants::kZombieDamageAcceleration);
      SetHealth(constants::kZombieHealth);
      SetDamage(constants::kZombieDamage);
      SetType(MovingObject::Type::kMob);
      SetStrategy(std::make_shared<BasicStrategy>());
      break;
    case Type::kZombieLord:
      SetWalkAcceleration(constants::kZombieLordWalkAcceleration);
      SetWalkMaxSpeed(constants::kZombieLordWalkMaxSpeed);
      SetWalkMaxAirAcceleration(constants::kZombieLordWalkMaxAirAcceleration);
      SetJumpSpeed(constants::kZombieLordJumpSpeed);
      SetDamageAcceleration(constants::kZombieLordDamageAcceleration);
      SetHealth(constants::kZombieLordHealth);
      SetDamage(constants::kZombieLordDamage);
      SetType(MovingObject::Type::kMob);
      SetStrategy(std::make_shared<BasicStrategy>());
      break;
    default:
      break;
  }
}

void Mob::MoveMob(double time) {
  strategy_->SetMobState(
      MobState(GetPosition(), GetSize(),
               kMobDamageAccelerations[static_cast<int>(GetId())],
               kMobJumps[static_cast<int>(GetId())], GetDamage(), IsOnGround(),
               IsOnCeil(), IsPushesLeft(), IsPushesRight()));
  strategy_->Update(time);
  UpdateState(strategy_->GetKeys(),
              time);  // maybe better to call MovingObject::Move
}
