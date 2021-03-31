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
  const Block& GetBlock(QPoint pos) override;
  void SetBlock(QPoint pos, Block block) override;

  const Chunk& GetChunk(QPoint chunk_pos);

  static void GetChunkCoords(QPoint* pos, QPoint* chunk_pos);
  static void GetChunkCoords(QPointF pos, QPoint* chunk_pos);
  static QPointF GetWorldCoords(QPoint chunk_pos);

  void UseChunk(QPoint chunk_pos);

 private:
  struct MapNode {
    Chunk chunk;
    bool is_used;
  };

  static constexpr int kClearTimeMSec = 1000;

  explicit ChunkMap(AbstractRegionGenerator* generator);

  void ClearUnusedChunks();

  Chunk* FindChunk(QPoint chunk_pos);

  std::vector<MapNode> nodes_;
  std::unique_ptr<AbstractRegionGenerator> generator_;
  QTimer clear_timer_;
};

#endif  // MODEL_CHUNK_MAP_H_
