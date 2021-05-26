#ifndef MODEL_FLAT_CHUNK_MAP_MANAGER_H_
#define MODEL_FLAT_CHUNK_MAP_MANAGER_H_

#include "model/abstract_map.h"
#include "model/abstract_map_manager.h"
#include "model/abstract_region_generator.h"
#include "model/chunk_map.h"

class FlatChunkMapManager : public AbstractMapManager {
 public:
  explicit FlatChunkMapManager(uint32_t seed);
  AbstractMap* GenerateMap(const QString& save_file) override;

 private:
  class FlatRegionGenerator : public AbstractRegionGenerator {
   public:
    explicit FlatRegionGenerator(uint32_t seed);

    Chunk Generate(const QString& save_file, QPoint chunk_pos) override;

   private:
    uint32_t GetChunkSeed(QPoint chunk_pos) const;

    static constexpr int kSeedMod = 2000000773;
    uint32_t seed_;
  };

  uint32_t seed_;
};

#endif  // MODEL_FLAT_CHUNK_MAP_MANAGER_H_
