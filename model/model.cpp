#include "model.h"

Model* Model::GetInstance() {
  static Model model;
  return &model;
}

std::shared_ptr<const Map> Model::GetMap() const { return map_; }

void Model::SetMap(std::shared_ptr<Map> map) { map_ = map; }

std::shared_ptr<const Player> Model::GetPlayer() const { return player_; }

void Model::SetPlayer(std::shared_ptr<Player> player) { player_ = player; }
