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
    enum class Biome { kIcePlains, kPlains, kDesert, kTypesCount };
    static constexpr int kTypesCount = static_cast<int>(Biome::kTypesCount);

    static constexpr int32_t kUpperChunk = Chunk::kHeight;

    explicit PerlinRegionGenerator(uint32_t seed);

    Chunk Generate(QPoint chunk_pos) override;

   private:
    static constexpr double kCavesRate = 0.3;
    static constexpr double kCavesScale = 8.5;

    static Biome GetBiome(double noise);

    void GenerateBasicBiome(
        QPoint chunk_pos, Chunk* chunk, int x,
        const std::array<int32_t, Chunk::kWidth>& height_map, Block upper,
        Block lower);
    void GenerateIcePlains(
        QPoint chunk_pos, Chunk* chunk, int x,
        const std::array<int32_t, Chunk::kWidth>& height_map);
    void GeneratePlains(QPoint chunk_pos, Chunk* chunk, int x,
                        const std::array<int32_t, Chunk::kWidth>& height_map);
    void GenerateDesert(QPoint chunk_pos, Chunk* chunk, int x,
                        const std::array<int32_t, Chunk::kWidth>& height_map);

    Chunk LandscapeGeneration(QPoint chunk_pos);

    void GenerateCaves(Chunk* chunk, QPoint chunk_pos);
    void GenerateOres(Chunk* chunk, QPoint chunk_pos);
    void GenerateUndergroundDecoration(Chunk* chunk, QPoint chunk_pos);

    double PerlinBiomeNoise(double seed);
    static double HeightNoise(double noise);
    static double StoneNoise(double noise);

    static constexpr double kBiomeLength = 3;

    static constexpr double kColdNoise = -0.35;
    static constexpr double kHotNoise = 0.35;

    static constexpr double kHillsRapidness = 3.5;
    static constexpr double kStoneRapidness = 2;
    static constexpr double kStoneMaxHeight = 8;

    static constexpr double kCoalMainRate = 0.18;  // vein precision
    static constexpr double kCoalRate = 0.50;      // amount in one vein
    static constexpr double kCoalScale = 10;

    static constexpr double kIronMainRate = 0.25;
    static constexpr double kIronRate = 0.55;
    static constexpr double kIronScale = 10;

    static constexpr double kShifiremondMainRate = 0.45;
    static constexpr double kShifiremondLowerRate = 0.2;
    static constexpr double kShifiremondUpperRate = 0.3;
    static constexpr double kShifiremondScale = 10;

    PerlinNoise1D noise_biome_temperature_;
    PerlinNoise1D noise_hills_;
    PerlinNoise1D noise_stone_;
    PerlinNoise2D noise_caves_;
    PerlinNoise2D noise_coal_;
    PerlinNoise2D noise_coal2_;
    PerlinNoise2D noise_iron_;
    PerlinNoise2D noise_iron2_;
    PerlinNoise2D noise_shimond_;
    PerlinNoise2D noise_shimond2_;
  };

  uint32_t seed_;
};
#endif  // MODEL_PERLIN_CHUNK_MAP_GENERATOR_H_
