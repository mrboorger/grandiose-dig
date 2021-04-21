#include "model/model.h"

#include <ctime>
#include <random>
#include <vector>

#include "utils.h"

Model* Model::GetInstance() {
  static Model model;
  return &model;
}

void Model::MoveObjects(
    const std::unordered_set<ControllerTypes::Key>& pressed_keys) {
  // TODO(Wind-Eagle): this is temporary code.
  if (player_->IsDead()) {
    exit(0);
  }
  std::vector<std::shared_ptr<Mob>> delete_mobs;
  for (auto i : mobs_) {
    if (i->IsDead()) {
      delete_mobs.push_back(i);
    }
  }
  for (auto i : delete_mobs) {
    mobs_.erase(i);
  }

  player_->Move(pressed_keys);

  static std::uniform_real_distribution<double> distrib(0.0, 1.0);
  for (auto mob : mobs_) {
    mob->MoveMob();
    if (!mob->RecentlyDamaged() &&
        distrib(utils::random) < constants::kMobSoundChance) {
      emit MobSound(mob->GetType());
    }
  }
}
