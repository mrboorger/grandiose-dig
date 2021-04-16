#include "model/model.h"

#include <vector>

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
  for (auto i : mobs_) {
    i->MoveMob();
  }
}
