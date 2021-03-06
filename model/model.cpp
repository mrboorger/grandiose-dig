#include "model.h"

Model* Model::GetInstance() {
  static Model model;
  return &model;
}

const Map* Model::GetMap() const {
  return map_;
}

void Model::SetMap(Map* map) {
  map_ = map;
}
