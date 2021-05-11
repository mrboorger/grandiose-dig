#include "model/mob.h"

namespace {

std::array<MobParameters, Mob::kTypesCount> kMobParameters = {
    constants::kZombieParameters, constants::kZombieLordParameters,
    constants::kQuioxParameters, constants::kQuioxParameters};
}  // namespace

Mob::Mob(QPointF pos, Type type)
    : MovingObject(pos, kMobParameters[static_cast<int>(type)].size_),
      mob_state_(),
      type_(type) {
  SetExternalId(static_cast<int>(type_));
  mob_state_.SetPos(GetPosition());
  mob_state_.SetSize(GetSize());
  mob_state_.SetJump(kMobParameters[static_cast<int>(type)].jump_in_blocks_);
  SetParamaters(kMobParameters[static_cast<int>(type)]);
  switch (type) {
    case Type::kZombie:
      SetType(MovingObject::Type::kMob);
      SetStrategy(std::make_shared<BasicStrategy>());
      break;
    case Type::kZombieLord:
      SetType(MovingObject::Type::kMob);
      SetStrategy(std::make_shared<ZombieSummonerStrategy>());
      break;
    case Type::kQuiox:
      SetType(MovingObject::Type::kMob);
      SetStrategy(std::make_shared<BasicStrategy>());
      break;
    case Type::kMagicQuiox:
      SetType(MovingObject::Type::kMob);
      SetStrategy(std::make_shared<MagicStrategy>());
      break;
    default:
      assert(false);
  }
}

void Mob::MoveMob(double time) {
  MobState mob_state;
  mob_state.SetPos(GetPosition());
  mob_state.SetSize(GetSize());
  mob_state.SetDamageAcceleration(
      kMobParameters[static_cast<int>(type_)].damage_acceleration_);
  mob_state.SetJump(kMobParameters[static_cast<int>(type_)].jump_in_blocks_);
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
