#ifndef MODEL_PERLIN_CHUNK_MAP_GENERATOR_H_
#define MODEL_PERLIN_CHUNK_MAP_GENERATOR_H_

#include "model/abstract_map_generator.h"
#include "model/abstract_region_generator.h"
#include "model/perlin_noise1d.h"
#include "model/chunk_map.h"

class PerlinChunkMapGenerator : public AbstractMapGenerator {
 public:
  explicit PerlinChunkMapGenerator(uint32_t seed);
  AbstractMap* GenerateMap() override;

 private:
  class PerlinRegionGenerator : public AbstractRegionGenerator {
   public:
    explicit PerlinRegionGenerator(uint32_t seed);

    Chunk Generate(QPoint chunk_pos) override;

   private:
    PerlinNoise1D noise_;
  };

  uint32_t seed_;
};
#endif  // MODEL_PERLIN_CHUNK_MAP_GENERATOR_H_
