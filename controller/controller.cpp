#include "controller.h"

Controller* Controller::GetInstance() {
  static Controller controller;
  return &controller;
}

void Controller::SetMap(AbstractMapGenerator* generator) {
  delete Model::GetInstance()->GetMap();
  Model::GetInstance()->SetMap(new Map(generator->GenerateMap()));
}
