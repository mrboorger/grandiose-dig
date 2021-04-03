#include "model/chunk_map.h"

#include <utility>

ChunkMap::ChunkMap(AbstractRegionGenerator* generator)
    : nodes_(), generator_(generator), clear_timer_() {
  auto clear_unused_chunks = [this]() { ClearUnusedChunks(); };
  clear_timer_.callOnTimeout(clear_unused_chunks);
  clear_timer_.start(kClearTimeMSec);
}

const Block& ChunkMap::GetBlock(QPoint pos) {
  QPoint chunk_pos{0, 0};
  GetChunkCoords(&pos, &chunk_pos);
  return FindChunk(chunk_pos)->GetBlock(pos);
}

void ChunkMap::SetBlock(QPoint pos, Block block) {
  QPoint chunk_pos{0, 0};
  GetChunkCoords(&pos, &chunk_pos);
  FindChunk(chunk_pos)->SetBlock(pos, block);
}

const Chunk& ChunkMap::GetChunk(QPoint chunk_pos) {
  return *FindChunk(chunk_pos);
}

void ChunkMap::GetChunkCoords(QPoint* pos, QPoint* chunk_pos) {
  std::tie(chunk_pos->rx(), pos->rx()) =
      utils::ArithmeticalDivMod(pos->x(), Chunk::kWidth);
  std::tie(chunk_pos->ry(), pos->ry()) =
      utils::ArithmeticalDivMod(pos->y(), Chunk::kHeight);
}

void ChunkMap::GetChunkCoords(QPointF pos, QPoint* chunk_pos) {
  QPoint casted{static_cast<int32_t>(pos.x()), static_cast<int32_t>(pos.y())};
  GetChunkCoords(&casted, chunk_pos);
}

QPointF ChunkMap::GetWorldCoords(QPoint chunk_pos) {
  return QPointF(chunk_pos.x() * Chunk::kWidth, chunk_pos.y() * Chunk::kHeight);
}

void ChunkMap::UseChunk(QPoint chunk_pos) { FindChunk(chunk_pos); }

void ChunkMap::ClearUnusedChunks() {
  for (auto i = nodes_.begin(); i != nodes_.end();) {
    if (!i->second.is_used) {
      i = nodes_.erase(i);
    } else {
      i->second.is_used = false;
      ++i;
    }
  }
}

Chunk* ChunkMap::FindChunk(QPoint chunk_pos) {
  if (!nodes_.count(chunk_pos)) {
    return &nodes_
                .emplace(chunk_pos,
                         MapNode{generator_->Generate(chunk_pos), true})
                .first->second.chunk;
  }
  auto& node = nodes_.at(chunk_pos);
  node.is_used = true;
  return &node.chunk;
}
