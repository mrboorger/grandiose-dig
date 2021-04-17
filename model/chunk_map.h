#ifndef MODEL_CHUNK_MAP_H_
#define MODEL_CHUNK_MAP_H_

#include <QPoint>
#include <QPointF>
#include <QTimer>
#include <map>
#include <memory>
#include <utility>

#include "model/abstract_map.h"
#include "model/abstract_region_generator.h"
#include "model/chunk.h"
#include "model/clearable_cache.h"
#include "utils.h"

class ChunkMap : public AbstractMap {
  friend class FlatChunkMapGenerator;
  friend class PerlinChunkMapGenerator;

 public:
  void SetBlock(QPoint pos, Block block) override;
  void CacheRegion(const QRect& region) override;

  const Chunk& GetChunk(QPoint chunk_pos);

  static std::pair<QPoint, QPoint> GetChunkCoords(QPoint pos);
  static QPoint GetChunkCoords(QPointF pos);
  static QPointF GetWorldCoords(QPoint chunk_pos);

 private:
  explicit ChunkMap(AbstractRegionGenerator* generator);

  Block* GetChangeableBlock(QPoint pos) override;

  Chunk& GetChunkMutable(QPoint chunk_pos);

  using NodesContainer =
      containers::ClearableCache<QPoint, Chunk,
                                 utils::QPointLexicographicalCompare>;

  NodesContainer nodes_;
  std::unique_ptr<AbstractRegionGenerator> generator_;
};

#endif  // MODEL_CHUNK_MAP_H_
