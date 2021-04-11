#ifndef MODEL_ABSTRACT_MAP_GENERATOR_H_
#define MODEL_ABSTRACT_MAP_GENERATOR_H_

#include "model/abstract_map.h"

class AbstractMapGenerator {
 public:
  AbstractMapGenerator() = default;
  AbstractMapGenerator(const AbstractMapGenerator&) = default;
  AbstractMapGenerator(AbstractMapGenerator&&) = default;

  virtual ~AbstractMapGenerator() = default;

  AbstractMapGenerator& operator=(const AbstractMapGenerator&) = default;
  AbstractMapGenerator& operator=(AbstractMapGenerator&&) = default;

  virtual AbstractMap* GenerateMap() = 0;
};

#endif  // MODEL_ABSTRACT_MAP_GENERATOR_H_
