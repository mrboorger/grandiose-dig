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
  SetType(MovingObject::Type::kMob);
  mob_state_.SetPos(GetPosition());
  mob_state_.SetSize(GetSize());
  mob_state_.SetJump(kMobParameters[static_cast<int>(type)].jump_in_blocks_);
  SetParamaters(kMobParameters[static_cast<int>(type)]);
  switch (type) {
    case Type::kZombie:
      SetStrategy(std::make_shared<BasicStrategy>());
      break;
    case Type::kZombieLord:
      SetStrategy(std::make_shared<ZombieSummonerStrategy>());
      break;
    case Type::kQuiox:
      SetStrategy(std::make_shared<BasicStrategy>());
      break;
    case Type::kMagicQuiox:
      SetStrategy(std::make_shared<MagicStrategy>());
      break;
    default:
      assert(false);
  }
}

QPointF Mob::GetMobSize(int id) { return kMobParameters[id].size_; }

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

void Mob::Read(const QJsonObject& json) {
  MovingObject::Read(json);

  mob_state_.Read(json["mob_state"].toObject());
  type_ = static_cast<Type>(json["type"].toInt());

  QString strategy_type =
      json["strategy"].toObject()["strategy_type"].toString();
  if (strategy_type == "BasicStrategy") {
    strategy_ = std::make_shared<BasicStrategy>();
  } else if (strategy_type == "ZombieSummonerStrategy") {
    strategy_ = std::make_shared<ZombieSummonerStrategy>();
  } else if (strategy_type == "MagicStrategy") {
    strategy_ = std::make_shared<MagicStrategy>();
  }
  strategy_->Read(json["strategy"].toObject());
}

void Mob::Write(QJsonObject* json) const {
  MovingObject::Write(json);

  QJsonObject mob_state;
  mob_state_.Write(&mob_state);
  (*json)["mob_state"] = mob_state;
  (*json)["type"] = static_cast<int>(type_);

  QJsonObject strategy;
  strategy_->Write(&strategy);
  (*json)["strategy"] = strategy;
}
