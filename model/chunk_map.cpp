#include "model/chunk_map.h"

#include <utility>

#include "model/constants.h"

ChunkMap::ChunkMap(AbstractRegionGenerator* generator)
    : nodes_(constants::kDefaultClearTimeMSec, GenChunk(generator)),
      generator_(generator) {}

Block* ChunkMap::GetBlockMutable(QPoint pos) {
  return nodes_.GetMutableValue(pos);
}

void ChunkMap::SetBlock(QPoint pos, Block block) {
  nodes_.SetValue(pos, block);
}

void ChunkMap::CacheRegion(const QRect& region) {
  nodes_.MarkUsedOrInsert(region);
}

// TODO(yaroslaffb): implement saving
void ChunkMap::Read(const QJsonObject& json) { Q_UNUSED(json); }

void ChunkMap::Write(QJsonObject& json) const { Q_UNUSED(json); }
