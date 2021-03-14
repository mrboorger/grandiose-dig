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

bool Controller::IsPressed(ControllerTypes::Key key) {
  return (pressed_keys_.find(key) != pressed_keys_.end());
}

ControllerTypes::Key Controller::TranslateKeyCode(int key_code) {
  switch (key_code) {
    case Qt::Key::Key_Left:
      return ControllerTypes::Key::kLeft;
    case Qt::Key::Key_Right:
      return ControllerTypes::Key::kRight;
    case Qt::Key::Key_Space:
      return ControllerTypes::Key::kJump;
    default:
      return ControllerTypes::Key::kUnused;
  }
}

void Controller::KeyPress(int key) {
  pressed_keys_.insert(TranslateKeyCode(key));
}

void Controller::KeyRelease(int key) {
  pressed_keys_.erase(TranslateKeyCode(key));
}
