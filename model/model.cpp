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
    const std::unordered_set<ControllerTypes::Key>& pressed_keys, double time) {
  // TODO(Wind-Eagle): this is temporary code.
  if (player_->IsDead()) {
    exit(0);
  }
  for (auto i = mobs_.begin(), last = mobs_.end(); i != last;) {
    if ((*i)->IsDead()) {
      i = mobs_.erase(i);
    } else {
      i++;
    }
  }

  player_->Move(pressed_keys, time);

  static std::uniform_real_distribution<double> distrib(0.0, 1.0);
  for (auto mob : mobs_) {
    mob->MoveMob(time);
    if (!mob->RecentlyDamaged() &&
        distrib(utils::random) < constants::kZombieSoundChance) {
      emit MobSound(mob->GetType());
    }
  }
}
