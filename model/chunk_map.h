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
#include "model/region_cache.h"
#include "model/chunk.h"
#include "utils.h"

class ChunkMap : public AbstractMap {
  friend class FlatChunkMapGenerator;
  friend class PerlinChunkMapGenerator;

 public:
  explicit ChunkMap(AbstractRegionGenerator* generator);

 private:
  class GenChunk {
   public:
    explicit GenChunk(AbstractRegionGenerator* generator)
        : generator_(generator) {}
    Chunk operator()(QPoint pos) { return generator_->Generate(pos); }

   private:
    AbstractRegionGenerator* generator_;
  };
  using NodesContainer =
      containers::RegionCache<Block, Chunk::kWidth, Chunk::kHeight,
                                         Chunk, GenChunk>;

  void SetBlockImpl(QPoint pos, Block block) override;
  void CacheRegionImpl(const QRect& region) override;

  Block* GetBlockMutableImpl(QPoint pos) override;

  NodesContainer nodes_;
  std::unique_ptr<AbstractRegionGenerator> generator_;
};

#endif  // MODEL_CHUNK_MAP_H_
