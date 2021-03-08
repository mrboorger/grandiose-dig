#ifndef CONTROLLER_CONTROLLER_H_
#define CONTROLLER_CONTROLLER_H_

#include <QKeyEvent>
#include <QTimer>
#include <unordered_set>

#include "model/abstract_map_generator.h"
#include "model/model.h"
#include "view/view.h"

class Controller : public QWidget {
 public:
  static Controller* GetInstance();

  Controller(const Controller&) = delete;
  Controller(Controller&&) = delete;

  ~Controller() = default;

  Controller& operator=(const Controller&) = delete;
  Controller& operator=(Controller&&) = delete;

  void SetGeneratedMap(AbstractMapGenerator* generator);

  bool IsPressed(int key);

 private:
  Controller();

  void timerEvent(QTimerEvent* event);

  void keyPressedEvent(QKeyEvent* event);
  void keyReleaseEvent(QKeyEvent* event);

  int timer_id_;
  std::unordered_set<int> pressed_keys_;
};

#endif  // CONTROLLER_CONTROLLER_H_
