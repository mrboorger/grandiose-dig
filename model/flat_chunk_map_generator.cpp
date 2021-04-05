#include "model/flat_chunk_map_generator.h"

#include <random>

AbstractMap* FlatChunkMapGenerator::GenerateMap() {
  return new ChunkMap(new FlatRegionGenerator(seed_));
}

FlatChunkMapGenerator::FlatChunkMapGenerator(uint32_t seed) : seed_(seed) {}

FlatChunkMapGenerator::FlatRegionGenerator::FlatRegionGenerator(uint32_t seed)
    : seed_(seed) {}

uint32_t FlatChunkMapGenerator::FlatRegionGenerator::GetChunkSeed(
    QPoint chunk_pos) const {
  return (seed_ + 57873833LL * chunk_pos.x() + 43947077LL * chunk_pos.y()) %
         kSeedMod;
}

Chunk FlatChunkMapGenerator::FlatRegionGenerator::Generate(QPoint chunk_pos) {
  Chunk chunk;
  std::mt19937 gen(GetChunkSeed(chunk_pos));
  if (chunk_pos.y() > 1) {
    for (int32_t y = 0; y < Chunk::kHeight; ++y) {
      for (int32_t x = 0; x < Chunk::kWidth; ++x) {
        chunk.SetBlock(QPoint(x, y), Block(Block::Type::kDirt));
      }
    }
  } else {
    for (int32_t y = 0; y < Chunk::kHeight; ++y) {
      for (int32_t x = 0; x < Chunk::kWidth; ++x) {
        if (gen() % 20 == 0) {
          chunk.SetBlock(QPoint(x, y), Block(Block::Type::kDirt));
        }
      }
    }
  }
  return chunk;
}
