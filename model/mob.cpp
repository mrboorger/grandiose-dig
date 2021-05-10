#include "model/mob.h"

namespace {

std::array<QPointF, Mob::kTypesCount> kMobSizes = {
    constants::kZombieSize, constants::kZombieLordSize, constants::kQuioxSize,
    constants::kQuioxSize};

std::array<QPoint, Mob::kTypesCount> kMobJumps = {
    constants::kZombieJumpInBlocks, constants::kZombieLordJumpInBlocks,
    constants::kQuioxJumpInBlocks, constants::kQuioxJumpInBlocks};

std::array<QPointF, Mob::kTypesCount> kMobDamageAccelerations = {
    constants::kZombieDamageAcceleration,
    constants::kZombieLordDamageAcceleration,
    constants::kQuioxDamageAcceleration, constants::kQuioxDamageAcceleration};

}  // namespace

Mob::Mob(QPointF pos, Type type)
    : MovingObject(pos, kMobSizes[static_cast<int>(type)]),
      mob_state_(),
      type_(type) {
  mob_state_.SetPos(GetPosition());
  mob_state_.SetSize(GetSize());
  mob_state_.SetJump(kMobJumps[static_cast<int>(type)]);
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
      SetStrategy(std::make_shared<ZombieSummonerStrategy>());
      break;
    case Type::kQuiox:
      SetWalkAcceleration(constants::kQuioxWalkAcceleration);
      SetWalkMaxSpeed(constants::kQuioxWalkMaxSpeed);
      SetWalkMaxAirAcceleration(constants::kQuioxWalkMaxAirAcceleration);
      SetJumpSpeed(constants::kQuioxJumpSpeed);
      SetDamageAcceleration(constants::kQuioxDamageAcceleration);
      SetHealth(constants::kQuioxHealth);
      SetDamage(constants::kQuioxDamage);
      SetType(MovingObject::Type::kMob);
      SetStrategy(std::make_shared<BasicStrategy>());
      break;
    case Type::kMagicQuiox:
      SetWalkAcceleration(constants::kQuioxWalkAcceleration);
      SetWalkMaxSpeed(constants::kQuioxWalkMaxSpeed);
      SetWalkMaxAirAcceleration(constants::kQuioxWalkMaxAirAcceleration);
      SetJumpSpeed(constants::kQuioxJumpSpeed);
      SetDamageAcceleration(constants::kQuioxDamageAcceleration);
      SetHealth(constants::kMagicQuioxHealth);
      SetDamage(constants::kMagicQuioxDamage);
      SetType(MovingObject::Type::kMob);
      SetStrategy(std::make_shared<MagicStrategy>());
      break;
    default:
      break;
  }
}

void Mob::MoveMob(double time) {
  MobState mob_state;
  mob_state.SetPos(GetPosition());
  mob_state.SetSize(GetSize());
  mob_state.SetDamageAcceleration(
      kMobDamageAccelerations[static_cast<int>(GetId())]);
  mob_state.SetJump(kMobJumps[static_cast<int>(GetId())]);
  mob_state.SetDamageTime(GetDamageTime());
  mob_state.SetDamage(GetDamage());
  mob_state.SetGroundTouch(IsOnGround());
  mob_state.SetCeilTouch(IsOnCeil());
  mob_state.SetLeftTouch(IsPushesLeft());
  mob_state.SetRightTouch(IsPushesRight());
  strategy_->SetMobState(mob_state);
  strategy_->Update(time);
  UpdateState(strategy_->GetKeys(),
              time);  // maybe better to call MovingObject::Move
}
