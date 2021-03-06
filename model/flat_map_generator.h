#ifndef MODEL_FLAT_MAP_GENERATOR_H_
#define MODEL_FLAT_MAP_GENERATOR_H_

#include "abstract_map_generator.h"

class FlatMapGenerator : public AbstractMapGenerator {
 public:
  Map GenerateMap() override;
 private:
};

#endif  // MODEL_FLAT_MAP_GENERATOR_H_
