#include "controller.h"

#include <memory>

#include "model/constants.h"

Controller* Controller::GetInstance() {
  static Controller controller;
  return &controller;
}

void Controller::SetGeneratedMap(AbstractMapGenerator* generator) {
  Model::GetInstance()->SetMap(std::make_shared<Map>(generator->GenerateMap()));
}

Controller::Controller() : timer_id_(startTimer(constants::kTick)) {}

void Controller::timerEvent(QTimerEvent* event) {
  Q_UNUSED(event);

  View::GetInstance()->repaint();
}

bool Controller::IsPressed(int key) {
  return (pressed_keys_.find(key) != pressed_keys_.end());
}
void Controller::keyPressedEvent(QKeyEvent* event) {
  pressed_keys_.insert(event->key());
}

void Controller::keyReleaseEvent(QKeyEvent* event) {
  pressed_keys_.erase(event->key());
}
