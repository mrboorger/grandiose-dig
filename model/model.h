#ifndef MODEL_MODEL_H_
#define MODEL_MODEL_H_

#include "map.h"

class Model {
 public:
  static Model* GetInstance();

  const Map* GetMap() const;
  void SetMap(Map* map);

 private:
  Model() = default;

  Map* map_;
};

#endif  // MODEL_MODEL_H_
