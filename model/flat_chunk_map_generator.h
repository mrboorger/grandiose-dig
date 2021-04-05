#ifndef MODEL_FLAT_CHUNK_MAP_GENERATOR_H_
#define MODEL_FLAT_CHUNK_MAP_GENERATOR_H_

#include "model/abstract_map.h"
#include "model/abstract_map_generator.h"
#include "model/abstract_region_generator.h"
#include "model/chunk_map.h"

class FlatChunkMapGenerator : public AbstractMapGenerator {
 public:
  explicit FlatChunkMapGenerator(uint32_t seed);
  AbstractMap* GenerateMap() override;

 private:
  class FlatRegionGenerator : public AbstractRegionGenerator {
   public:
    explicit FlatRegionGenerator(uint32_t seed);

    Chunk Generate(QPoint chunk_pos) override;

   private:
    uint32_t GetChunkSeed(QPoint chunk_pos) const;

    static constexpr int kSeedMod = 2000000773;
    uint32_t seed_;
  };

  uint32_t seed_;
};

#endif  // MODEL_FLAT_CHUNK_MAP_GENERATOR_H_
