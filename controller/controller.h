#ifndef CONTROLLER_CONTROLLER_H_
#define CONTROLLER_CONTROLLER_H_

#include "abstract_controller.h"

#include "model/abstract_map_generator.h"
#include "model/model.h"

class Controller : public AbstractController {
 public:
  static Controller* GetInstance();

  void SetMap(AbstractMapGenerator* generator);
 private:
  Controller() = default;
};

#endif  // CONTROLLER_CONTROLLER_H_
