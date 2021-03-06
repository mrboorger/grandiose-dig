#ifndef MODEL_ABSTRACT_MAP_GENERATOR_H_
#define MODEL_ABSTRACT_MAP_GENERATOR_H_

#include "map.h"

class AbstractMapGenerator {
 public:
  virtual Map GenerateMap() = 0;
 private:
};

#endif  // MODEL_ABSTRACT_MAP_GENERATOR_H_
