#ifndef CONTROLLER_CONTROLLER_H_
#define CONTROLLER_CONTROLLER_H_

#include <QKeyEvent>
#include <QTimer>
#include <cstdint>
#include <unordered_set>

#include "controller/controller_types.h"
#include "model/abstract_map_generator.h"
#include "model/model.h"
#include "model/player.h"
#include "view/view.h"

class Controller {
 public:
  static Controller* GetInstance();

  Controller(const Controller&) = delete;
  Controller(Controller&&) = delete;

  ~Controller() = default;

  Controller& operator=(const Controller&) = delete;
  Controller& operator=(Controller&&) = delete;

  void SetGeneratedMap(AbstractMapGenerator* generator);

  void SetPlayer();
  void SetMob();

  static ControllerTypes::Key TranslateKeyCode(int key_code);

  void KeyPress(int key);
  void KeyRelease(int key);

 private:
  Controller();

  void TickEvent();

  QTimer tick_timer_;
  std::unordered_set<ControllerTypes::Key> pressed_keys_;
};

#endif  // CONTROLLER_CONTROLLER_H_
