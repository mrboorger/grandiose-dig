#include "model/flat_chunk_map_manager.h"

#include <random>

AbstractMap* FlatChunkMapManager::GenerateMap(const QString& save_file) {
  return new ChunkMap(save_file, new FlatRegionGenerator(seed_));
}

FlatChunkMapManager::FlatChunkMapManager(uint32_t seed) : seed_(seed) {}

FlatChunkMapManager::FlatRegionGenerator::FlatRegionGenerator(uint32_t seed)
    : seed_(seed) {}

uint32_t FlatChunkMapManager::FlatRegionGenerator::GetChunkSeed(
    QPoint chunk_pos) const {
  return (seed_ + 57873833LL * chunk_pos.x() + 43947077LL * chunk_pos.y()) %
         kSeedMod;
}

Chunk FlatChunkMapManager::FlatRegionGenerator::Generate(
    const QString& save_file, QPoint chunk_pos) {
  Chunk chunk;
  std::mt19937 gen(GetChunkSeed(chunk_pos));
  // TODO(Wind-Eagle): This is temporary code
  for (int32_t y = 0; y < Chunk::kHeight; ++y) {
    for (int32_t x = 0; x < Chunk::kWidth; ++x) {
      if (chunk_pos.y() > 1 || gen() % 20 == 0) {
        if (chunk_pos.y() == 2 && y == 0) {
          chunk.SetBlock(QPoint(x, y), Block(Block::Type::kGrass));
        } else {
          chunk.SetBlock(QPoint(x, y), Block(Block::Type::kDirt));
        }
      }
    }
  }
  return chunk;
}
