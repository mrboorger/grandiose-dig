#ifndef MODEL_PERLIN_CHUNK_MAP_MANAGER_H_
#define MODEL_PERLIN_CHUNK_MAP_MANAGER_H_

#include "model/abstract_map_manager.h"
#include "model/abstract_region_generator.h"
#include "model/chunk_map.h"
#include "model/perlin_noise1d.h"
#include "model/perlin_noise2d.h"

class PerlinChunkMapManager : public AbstractMapManager {
 public:
  explicit PerlinChunkMapManager(uint32_t seed);
  AbstractMap* GenerateMap(const QString& save_file) override;

 private:
  class PerlinRegionGenerator : public AbstractRegionGenerator {
   public:
    static constexpr int32_t kUpperChunk = Chunk::kHeight;

    explicit PerlinRegionGenerator(uint32_t seed);

    Chunk Generate(const QString& save_file, QPoint chunk_pos) override;

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
#endif  // MODEL_PERLIN_CHUNK_MAP_MANAGER_H_
