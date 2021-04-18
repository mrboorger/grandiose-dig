#include "model/chunk_map.h"

#include <utility>

#include "model/constants.h"

ChunkMap::ChunkMap(AbstractRegionGenerator* generator)
    : nodes_(constants::kDefaultClearTimeMSec), generator_(generator) {}

Block* ChunkMap::GetBlockMutable(QPoint pos) {
  QPoint chunk_pos{0, 0};
  std::tie(chunk_pos, pos) = GetChunkCoords(pos);
  return GetChunkMutable(chunk_pos).GetBlockMutable(pos);
}

void ChunkMap::SetBlock(QPoint pos, Block block) {
  QPoint chunk_pos{0, 0};
  std::tie(chunk_pos, pos) = GetChunkCoords(pos);
  GetChunkMutable(chunk_pos).SetBlock(pos, block);
}

void ChunkMap::CacheRegion(const QRect& region) {
  QPoint start = GetChunkCoords(region.topLeft()).first;
  QPoint finish = GetChunkCoords(region.bottomRight()).first;
  for (int chunk_x = start.x(); chunk_x <= finish.x(); ++chunk_x) {
    for (int chunk_y = start.y(); chunk_y <= finish.y(); ++chunk_y) {
      GetChunkMutable(QPoint(chunk_x, chunk_y));
    }
  }
}

const Chunk& ChunkMap::GetChunk(QPoint chunk_pos) {
  return GetChunkMutable(chunk_pos);
}

std::pair<QPoint, QPoint> ChunkMap::GetChunkCoords(QPoint pos) {
  QPoint chunk_pos{0, 0};
  std::tie(chunk_pos.rx(), pos.rx()) =
      utils::ArithmeticalDivMod(pos.x(), Chunk::kWidth);
  std::tie(chunk_pos.ry(), pos.ry()) =
      utils::ArithmeticalDivMod(pos.y(), Chunk::kHeight);
  return std::make_pair(chunk_pos, pos);
}

QPoint ChunkMap::GetChunkCoords(QPointF pos) {
  QPoint casted{static_cast<int32_t>(pos.x()), static_cast<int32_t>(pos.y())};
  return GetChunkCoords(casted).first;
}

QPointF ChunkMap::GetWorldCoords(QPoint chunk_pos) {
  return QPointF(chunk_pos.x() * Chunk::kWidth, chunk_pos.y() * Chunk::kHeight);
}

Chunk& ChunkMap::GetChunkMutable(QPoint chunk_pos) {
  auto found = nodes_.Get(chunk_pos);
  if (!found) {
    return nodes_.Insert(chunk_pos, generator_->Generate(chunk_pos));
  }
  return found.value();
}
