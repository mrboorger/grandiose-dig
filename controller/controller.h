#ifndef CONTROLLER_CONTROLLER_H_
#define CONTROLLER_CONTROLLER_H_

#include "abstract_controller.h"

class Controller : public AbstractController {
 public:
  static Controller* GetInstance();
 private:
  Controller() = default;
};

#endif  // CONTROLLER_CONTROLLER_H_