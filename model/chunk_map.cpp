#include "model/chunk_map.h"

#include <utility>

ChunkMap::ChunkMap(AbstractRegionGenerator* generator)
    : generator_(generator), last_used_(nodes_.end()) {}

const Block& ChunkMap::GetBlock(QPoint pos) {
  QPoint chunk_pos{0, 0};
  std::tie(chunk_pos, pos) = GetChunkCoords(pos);
  return FindChunk(chunk_pos)->GetBlock(pos);
}

void ChunkMap::SetBlock(QPoint pos, Block block) {
  QPoint chunk_pos{0, 0};
  std::tie(chunk_pos, pos) = GetChunkCoords(pos);
  FindChunk(chunk_pos)->SetBlock(pos, block);
}

void ChunkMap::CacheRegion(const QRect& region) {
  QPoint start = GetChunkCoords(region.topLeft()).first;
  QPoint finish = GetChunkCoords(region.bottomRight()).first;
  for (int chunk_x = start.x(); chunk_x <= finish.x(); ++chunk_x) {
    for (int chunk_y = start.y(); chunk_y <= finish.y(); ++chunk_y) {
      FindChunk(QPoint(chunk_x, chunk_y));
    }
  }
}

const Chunk& ChunkMap::GetChunk(QPoint chunk_pos) {
  return *FindChunk(chunk_pos);
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

Chunk* ChunkMap::FindChunk(QPoint chunk_pos) {
  if (nodes_.IsCleared()) {
    last_used_ = nodes_.end();
  }
  if (last_used_ == nodes_.end() || last_used_->first != chunk_pos) {
    if (!nodes_.count(chunk_pos)) {
      last_used_ = nodes_
                       .emplace(chunk_pos,
                                MapNode{generator_->Generate(chunk_pos), true})
                       .first;
    } else {
      last_used_ = nodes_.find(chunk_pos);
      last_used_->second.is_used = true;
    }
  }
  return &last_used_->second.chunk;
}
