#ifndef MODEL_FLAT_MAP_GENERATOR_H_
#define MODEL_FLAT_MAP_GENERATOR_H_

#include "model/abstract_map_generator.h"

class FlatMapGenerator : public AbstractMapGenerator {
 public:
  AbstractMap* GenerateMap() override;
};

#endif  // MODEL_FLAT_MAP_GENERATOR_H_
