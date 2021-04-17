#include "controller/controller.h"

#include <cmath>
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
      std::make_shared<Player>(QPointF(147.0, 126.0)));
  View::GetInstance()->SetInventoryDrawer(
      new InventoryDrawer(Model::GetInstance()->GetPlayer()->GetInventory()));
}

void Controller::SetMob() {
  // TODO(Wind-Eagle): this is temporary code.
  Model::GetInstance()->AddMob(
      std::make_shared<Mob>(QPointF(147.0, 126.0), QPointF(0.75, 1.75)));
}

void Controller::TickEvent() {
  Model::GetInstance()->MoveObjects(pressed_keys_);
  if (is_pressed_right_mouse_button) {
    QPoint block_coord = View::GetInstance()->GetBlockUnderCursorCoord();
    Model::GetInstance()->GetMap()->HitBlock(block_coord, 1);
    View::GetInstance()->UpdateBlock(block_coord);
  }
  View::GetInstance()->repaint();
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

void Controller::PickItemToPlayer(InventoryItem item) {
  Model::GetInstance()->PickItemToPlayer(item);
}

void Controller::ButtonPress(Qt::MouseButton button) {
  if (button == Qt::MouseButton::LeftButton) {
    is_pressed_right_mouse_button = true;
  }
}

void Controller::ButtonRelease(Qt::MouseButton button) {
  if (button == Qt::MouseButton::LeftButton) {
    is_pressed_right_mouse_button = false;
  }
}
