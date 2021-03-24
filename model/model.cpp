#include "model/model.h"

Model* Model::GetInstance() {
  static Model model;
  return &model;
}
