#include "controller/controller.h"

#include <memory>

#include "model/abstract_map_generator.h"
#include "model/chunk_map.h"
#include "model/constants.h"
#include "view/abstract_map_drawer.h"
#include "view/buffered_map_drawer.h"
#include "view/map_drawer.h"

Controller* Controller::GetInstance() {
  static Controller controller;
  return &controller;
}

void Controller::SetGeneratedMap(AbstractMapGenerator* generator) {
  auto map = std::shared_ptr<AbstractMap>(generator->GenerateMap());
  Model::GetInstance()->SetMap(map);
  View::GetInstance()->SetDrawer(new BufferedMapDrawer(map));
}

Controller::Controller() : tick_timer_() {
  tick_timer_.callOnTimeout([this]() { TickEvent(); });
  tick_timer_.start(constants::kTickDurationMsec);
}

void Controller::SetPlayer() {
  // TODO(Wind-Eagle): this is temporary code.
  Model::GetInstance()->SetPlayer(
      std::make_shared<Player>(QPointF(-3.0, 126.0)));
}

void Controller::SetMob() {
  // TODO(Wind-Eagle): this is temporary code.
  Model::GetInstance()->AddMob(
      std::make_shared<Mob>(QPointF(15, 126.25), QPointF(0.75, 1.75)));
}

void Controller::TickEvent() {
  if (pressed_keys_.count(ControllerTypes::Key::kExit)) {
    pressed_keys_.erase(ControllerTypes::Key::kExit);
    switch (View::GetInstance()->GetGameState()) {
      case GameState::kGame:
        View::GetInstance()->ChangeGameState(GameState::kPaused);
        break;
      case GameState::kMainMenu:
        View::Quit();
        break;
      case GameState::kPaused:
        View::GetInstance()->ChangeGameState(GameState::kMainMenu);
        break;
      case GameState::kSettings:
        View::GetInstance()->ChangeGameState(GameState::kSwitchingToPrevious);
      default:
        break;
    }
  } else if (View::GetInstance()->GetGameState() == GameState::kGame) {
    Model::GetInstance()->MoveObjects(pressed_keys_);
    View::GetInstance()->repaint();
  }
}

ControllerTypes::Key Controller::TranslateKeyCode(int key_code) {
  if (key_code == GetInstance()->settings_.value("kLeft").toInt()) {
    return ControllerTypes::Key::kLeft;
  }
  if (key_code == GetInstance()->settings_.value("kRight").toInt()) {
    return ControllerTypes::Key::kRight;
  }
  if (key_code == GetInstance()->settings_.value("kJump").toInt()) {
    return ControllerTypes::Key::kJump;
  }
  if (key_code == Qt::Key::Key_Escape || key_code == Qt::Key::Key_Exit ||
      key_code == Qt::Key::Key_Menu) {
    return ControllerTypes::Key::kExit;
  }
  return ControllerTypes::Key::kUnused;
}

void Controller::KeyPress(int key) {
  pressed_keys_.insert(TranslateKeyCode(key));
}

void Controller::KeyRelease(int key) {
  pressed_keys_.erase(TranslateKeyCode(key));
}
