#ifndef MODEL_CHUNK_MAP_H_
#define MODEL_CHUNK_MAP_H_

#include <QPoint>
#include <QPointF>
#include <QTimer>
#include <map>
#include <memory>

#include "model/abstract_map.h"
#include "model/abstract_region_generator.h"
#include "model/chunk.h"
#include "model/clearable_map.h"
#include "utils.h"

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

  explicit ChunkMap(AbstractRegionGenerator* generator);

  Chunk* FindChunk(QPoint chunk_pos);

  containers::ClearableMap<QPoint, MapNode, utils::QPointCompare> nodes_;
  std::unique_ptr<AbstractRegionGenerator> generator_;
};

#endif  // MODEL_CHUNK_MAP_H_
