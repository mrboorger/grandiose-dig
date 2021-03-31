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
  chunk_pos->setX(pos->x() / Chunk::kWidth);
  pos->setX(pos->x() % Chunk::kWidth);
  if (pos->x() < 0) {
    *pos += QPoint(Chunk::kWidth, 0);
  }
  chunk_pos->setY(pos->y() / Chunk::kHeight);
  pos->setY(pos->y() % Chunk::kHeight);
  if (pos->y() < 0) {
    *pos += QPoint(0, Chunk::kHeight);
  }
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
  for (int i = 0; i < static_cast<int>(nodes_.size()); ++i) {
    if (!nodes_[i].is_used) {
      std::swap(nodes_[i], nodes_.back());
      nodes_.pop_back();
      --i;
    } else {
      nodes_[i].is_used = false;
    }
  }
}

Chunk* ChunkMap::FindChunk(QPoint chunk_pos) {
  for (auto& [chunk, is_used] : nodes_) {
    if (chunk.GetPos() == chunk_pos) {
      is_used = true;
      return &chunk;
    }
  }
  nodes_.push_back(MapNode{generator_->Generate(chunk_pos), true});
  return &nodes_.back().chunk;
}
