#ifndef MODEL_PERLIN_CHUNK_MAP_GENERATOR_H_
#define MODEL_PERLIN_CHUNK_MAP_GENERATOR_H_

#include "model/abstract_map_generator.h"
#include "model/abstract_region_generator.h"
#include "model/chunk_map.h"
#include "model/perlin_noise1d.h"
#include "model/perlin_noise2d.h"

class PerlinChunkMapGenerator : public AbstractMapGenerator {
 public:
  explicit PerlinChunkMapGenerator(uint32_t seed);
  AbstractMap* GenerateMap() override;

 private:
  class PerlinRegionGenerator : public AbstractRegionGenerator {
   public:
    static constexpr int32_t kUpperChunk = Chunk::kHeight;

    explicit PerlinRegionGenerator(uint32_t seed);

    Chunk Generate(QPoint chunk_pos) override;

   private:
    static constexpr double kCavesRate = 0.3;
    static constexpr double kCavesScale = 8.5;

    Chunk LandscapeGeneration(QPoint chunk_pos);

    void GenerateCaves(Chunk* chunk, QPoint chunk_pos);
    void GenerateOres(Chunk* chunk, QPoint chunk_pos);

    static double HeightNoise(double noise);
    static double StoneNoise(double noise);

    static constexpr double kHillsRapidness = 3.5;
    static constexpr double kStoneRapidness = 2;
    static constexpr double kStoneMaxHeight = 8;

    static constexpr double kCoalMainRate = 0.1;  // vein precision
    static constexpr double kCoalRate = 0.35;     // amount in one vein
    static constexpr double kCoalScale = 10;

    static constexpr double kIronMainRate = 0.17;
    static constexpr double kIronRate = 0.4;
    static constexpr double kIronScale = 10;

    PerlinNoise1D noise_hills_;
    PerlinNoise1D noise_stone_;
    PerlinNoise2D noise_caves_;
    PerlinNoise2D noise_coal_;
    PerlinNoise2D noise_coal2_;
    PerlinNoise2D noise_iron_;
    PerlinNoise2D noise_iron2_;
  };

  uint32_t seed_;
};
#endif  // MODEL_PERLIN_CHUNK_MAP_GENERATOR_H_
