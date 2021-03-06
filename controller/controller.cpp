#include "controller.h"

Controller* Controller::GetInstance() {
  static Controller controller;
  return &controller;
}
