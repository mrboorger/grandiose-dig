#include "model/chunk_map.h"

#include <utility>

ChunkMap::ChunkMap(AbstractRegionGenerator* generator)
    : nodes_(), generator_(generator), clear_timer_() {
  auto clear_unused_chunks = [this]() {
    ClearUnusedChunks();
  };
  clear_timer_.callOnTimeout(clear_unused_chunks);
  clear_timer_.start(kClearTimeMSec);
}

const Block& ChunkMap::GetBlock(int32_t x, int32_t y) {
  int32_t chunk_x = 0;
  int32_t chunk_y = 0;
  GetChunkCoords(&x, &y, &chunk_x, &chunk_y);
  return FindChunk(chunk_x, chunk_y)->GetBlock(x, y);
}

void ChunkMap::SetBlock(int32_t x, int32_t y, Block block) {
  int32_t chunk_x = 0;
  int32_t chunk_y = 0;
  GetChunkCoords(&x, &y, &chunk_x, &chunk_y);
  FindChunk(chunk_x, chunk_y)->SetBlock(x, y, block);
}

const Chunk& ChunkMap::GetChunk(int32_t chunk_x, int32_t chunk_y) {
  return *FindChunk(chunk_x, chunk_y);
}

void ChunkMap::GetChunkCoords(int32_t* x, int32_t* y, int32_t* chunk_x,
                              int32_t* chunk_y) {
  *y = *y;
  *chunk_x = *x / Chunk::kWidth;
  *x %= Chunk::kWidth;
  if (*x < 0) {
    *x += Chunk::kWidth;
  }
  *chunk_y = *y / Chunk::kHeight;
  *y %= Chunk::kHeight;
  if (*y < 0) {
    *y += Chunk::kWidth;
  }
}

void ChunkMap::GetChunkCoords(float x, float y, int32_t* chunk_x,
                              int32_t* chunk_y) {
  int casted_x = x;
  int casted_y = y;
  GetChunkCoords(&casted_x, &casted_y, chunk_x, chunk_y);
}

QPointF ChunkMap::GetWorldCoords(int32_t chunk_x, int32_t chunk_y) {
  return QPointF(chunk_x * Chunk::kWidth, chunk_y * Chunk::kHeight);
}

void ChunkMap::UseChunk(int32_t chunk_x, int32_t chunk_y) {
  FindChunk(chunk_x, chunk_y);
}

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

Chunk* ChunkMap::FindChunk(int32_t chunk_x, int32_t chunk_y) {
  for (auto& [chunk, is_used] : nodes_) {
    if (chunk.GetPosX() == chunk_x && chunk.GetPosY() == chunk_y) {
      is_used = true;
      return &chunk;
    }
  }
  nodes_.push_back(MapNode{generator_->Generate(chunk_x, chunk_y), true});
  return &nodes_.back().chunk;
}
