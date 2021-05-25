#include "model/chunk_map.h"

#include <utility>

#include "model/constants.h"

ChunkMap::ChunkMap(const QString& save_file, AbstractRegionGenerator* generator)
    : nodes_(save_file, constants::kDefaultClearTimeMSec, GenChunk(generator)),
      generator_(generator) {}

Block* ChunkMap::GetBlockMutableImpl(QPoint pos) {
  return nodes_.GetMutableValue(pos);
}

void ChunkMap::SetBlockImpl(QPoint pos, Block block) {
  nodes_.SetValue(pos, block);
}

void ChunkMap::CacheRegionImpl(const QRect& region) {
  nodes_.MarkUsedOrInsert(region);
}
