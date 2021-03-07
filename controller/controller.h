#ifndef CONTROLLER_CONTROLLER_H_
#define CONTROLLER_CONTROLLER_H_

#include "model/abstract_map_generator.h"
#include "model/model.h"

class Controller {
 public:
  static Controller* GetInstance();

  Controller(const Controller&) = delete;
  Controller(Controller&&) = delete;

  void SetMap(AbstractMapGenerator* generator);

 private:
  Controller() = default;
};

#endif  // CONTROLLER_CONTROLLER_H_
