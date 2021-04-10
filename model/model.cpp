#include "model/model.h"

Model* Model::GetInstance() {
  static Model model;
  return &model;
}

void Model::MoveObjects(
    const std::unordered_set<ControllerTypes::Key>& pressed_keys) {
  player_->Move(pressed_keys);
  for (auto i : mobs_) {
    i->MoveMob();
  }
}
