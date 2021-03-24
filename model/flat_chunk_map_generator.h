#ifndef MODEL_FLAT_CHUNK_MAP_GENERATOR_H_
#define MODEL_FLAT_CHUNK_MAP_GENERATOR_H_

#include <random>

#include "model/abstract_map.h"
#include "model/abstract_map_generator.h"
#include "model/abstract_region_generator.h"
#include "model/chunk_map.h"

class FlatChunkMapGenerator : public AbstractMapGenerator {
 public:
  AbstractMap* GenerateMap() override;

 private:
  class FlatRegionGenerator : public AbstractRegionGenerator {
   public:
    explicit FlatRegionGenerator(int seed);

    Chunk Generate(int32_t chunk_x, int32_t chunk_y) override;

   private:
    std::mt19937 gen_;
  };

  static constexpr int kSeed = 42;
};

#endif  // MODEL_FLAT_CHUNK_MAP_GENERATOR_H_
