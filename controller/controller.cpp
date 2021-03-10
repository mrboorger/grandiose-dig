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

Controller::Controller()
    : timer_id_(startTimer(constants::kTickDurationMsec)) {}

void Controller::SetPlayer() {
  // TODO(Wind-Eagle): this is temporary code.
  Model::GetInstance()->SetPlayer(
      std::make_shared<Player>(QPointF(150, 148.25)));
}

void Controller::timerEvent(QTimerEvent* event) {
  Q_UNUSED(event);
  Model::GetInstance()->MoveObjects(pressed_keys_);
  View::GetInstance()->repaint();
}

bool Controller::IsPressed(int key) {
  return (pressed_keys_.find(key) != pressed_keys_.end());
}

void Controller::keyPressEvent(QKeyEvent* event) {
  pressed_keys_.insert(event->key());
}

void Controller::keyReleaseEvent(QKeyEvent* event) {
  pressed_keys_.erase(event->key());
}
