#include "controller.h"

#include <memory>

Controller* Controller::GetInstance() {
  static Controller controller;
  return &controller;
}

void Controller::SetMap(AbstractMapGenerator* generator) {
  Model::GetInstance()->SetMap(
      std::make_shared<Map>(Map(generator->GenerateMap())));
}
