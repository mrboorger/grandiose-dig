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
#include "model/buffered_clearable_cache.h"
#include "model/chunk.h"
#include "utils.h"

class ChunkMap : public AbstractMap {
  friend class FlatChunkMapGenerator;
  friend class PerlinChunkMapGenerator;

 public:
  void SetBlock(QPoint pos, Block block) override;
  void CacheRegion(const QRect& region) override;

  explicit ChunkMap(AbstractRegionGenerator* generator);

  void Read(const QJsonObject &json) override;
  void Write(QJsonObject &json) const override;

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
      containers::BufferedClearableCache<Block, Chunk::kWidth, Chunk::kHeight,
                                         Chunk, GenChunk>;

  Block* GetBlockMutable(QPoint pos) override;

  NodesContainer nodes_;
  std::unique_ptr<AbstractRegionGenerator> generator_;
};

#endif  // MODEL_CHUNK_MAP_H_
