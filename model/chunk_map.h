#ifndef MODEL_CHUNK_MAP_H_
#define MODEL_CHUNK_MAP_H_

#include <QJsonDocument>
#include <QPoint>
#include <QPointF>
#include <QTimer>
#include <map>
#include <memory>
#include <utility>

#include "model/abstract_map.h"
#include "model/abstract_region_generator.h"
#include "model/chunk.h"
#include "model/region_cache.h"
#include "utils.h"

class ChunkMap : public AbstractMap {
  friend class FlatChunkMapGenerator;
  friend class PerlinChunkMapGenerator;

 public:
  explicit ChunkMap(const QString& save_file,
                    AbstractRegionGenerator* generator);

 private:
  class GenChunk {
   public:
    explicit GenChunk(AbstractRegionGenerator* generator)
        : generator_(generator) {}
    Chunk operator()(const QString& save_file, QPoint pos);

   private:
    AbstractRegionGenerator* generator_;
  };
  class SaveChunk {
   public:
    SaveChunk() = default;
    void operator()(const QString& save_file, const QPoint& pos,
                    const Chunk& chunk);
  };
  using NodesContainer =
      containers::RegionCache<Block, Chunk::kWidth, Chunk::kHeight, Chunk,
                              GenChunk, SaveChunk>;

  void SetBlockImpl(QPoint pos, Block block) override;
  void CacheRegionImpl(const QRect& region) override;

  Block* GetBlockMutableImpl(QPoint pos) override;

  NodesContainer nodes_;
  std::unique_ptr<AbstractRegionGenerator> generator_;
};

#endif  // MODEL_CHUNK_MAP_H_
