#include "controller.h"

#include <memory>

#include "model/constants.h"

Controller* Controller::GetInstance() {
  static Controller controller;
  return &controller;
}

void Controller::SetMap(AbstractMapGenerator* generator) {
  Model::GetInstance()->SetMap(
      std::make_shared<Map>(Map(generator->GenerateMap())));
}

void Controller::SetPlayer() {
  Model::GetInstance()->SetPlayer(
      std::make_shared<Player>(Player(QPointF(150, 148.25))));
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
