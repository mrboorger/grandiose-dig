#ifndef MODEL_PERLIN_CHUNK_MAP_GENERATOR_H_
#define MODEL_PERLIN_CHUNK_MAP_GENERATOR_H_

#include "model/abstract_map_generator.h"
#include "model/abstract_region_generator.h"
#include "model/perlin_noise1d.h"
#include "model/perlin_noise2d.h"
#include "model/chunk_map.h"

class PerlinChunkMapGenerator : public AbstractMapGenerator {
 public:
  explicit PerlinChunkMapGenerator(uint32_t seed);
  AbstractMap* GenerateMap() override;

 private:
  class PerlinRegionGenerator : public AbstractRegionGenerator {
   public:
    static constexpr int32_t kUpperChunk = 1;

    explicit PerlinRegionGenerator(uint32_t seed);

    Chunk Generate(QPoint chunk_pos) override;

   private:
    static constexpr double kCavesRate = 0.2;
    static constexpr double kCavesScale = 10;
    Chunk BasicGeneration(QPoint chunk_pos);
    void GenerateCaves(Chunk* chunk, QPoint chunk_pos);

    PerlinNoise1D noise1d_;
    PerlinNoise2D noise2d_;
  };

  uint32_t seed_;
};
#endif  // MODEL_PERLIN_CHUNK_MAP_GENERATOR_H_
