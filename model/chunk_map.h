#ifndef MODEL_CHUNK_MAP_H_
#define MODEL_CHUNK_MAP_H_

#include <QPointF>
#include <QTimer>
#include <memory>
#include <vector>

#include "model/abstract_map.h"
#include "model/abstract_region_generator.h"
#include "model/chunk.h"

class ChunkMap : public AbstractMap {
  friend class FlatChunkMapGenerator;

 public:
  const Block& GetBlock(int32_t x, int32_t y) override;
  void SetBlock(int32_t x, int32_t y, Block block) override;

  const Chunk& GetChunk(int32_t chunk_x, int32_t chunk_y);

  static void GetChunkCoords(int32_t* x, int32_t* y, int32_t* chunk_x,
                             int32_t* chunk_y);
  static void GetChunkCoords(float x, float y, int32_t* chunk_x,
                             int32_t* chunk_y);
  static QPointF GetWorldCoords(int32_t chunk_x, int32_t chunk_y);

  void UseChunk(int32_t chunk_x, int32_t chunk_y);

 private:
  explicit ChunkMap(AbstractRegionGenerator* generator);

  static constexpr int kClearTimeMSec = 1000;

  struct MapNode {
    Chunk chunk;
    bool is_used;
  };

  void ClearUnusedChunks();

  Chunk* FindChunk(int32_t chunk_x, int32_t chunk_y);

  std::vector<MapNode> nodes_;
  std::unique_ptr<AbstractRegionGenerator> generator_;
  QTimer clear_timer_;
};

#endif  // MODEL_CHUNK_MAP_H_
