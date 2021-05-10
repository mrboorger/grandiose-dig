#include "model/perlin_chunk_map_generator.h"

#include <array>

#include "model/perlin_noise1d.h"
#include "utils.h"

AbstractMap* PerlinChunkMapGenerator::GenerateMap() {
  return new ChunkMap(new PerlinRegionGenerator(seed_));
}

PerlinChunkMapGenerator::PerlinChunkMapGenerator(uint32_t seed) : seed_(seed) {}

PerlinChunkMapGenerator::PerlinRegionGenerator::PerlinRegionGenerator(
    uint32_t seed)
    : noise1d_(seed), noise2d_(seed) {}

Chunk PerlinChunkMapGenerator::PerlinRegionGenerator::Generate(
    QPoint chunk_pos) {
  Chunk chunk = BasicGeneration(chunk_pos);
  GenerateCaves(&chunk, chunk_pos);
  return chunk;
}

Chunk PerlinChunkMapGenerator::PerlinRegionGenerator::BasicGeneration(
    QPoint chunk_pos) {
  Chunk chunk;
  if (chunk_pos.y() > kUpperChunk) {
    chunk.FillWith(Block(Block::Type::kDirt));
  } else if (chunk_pos.y() == kUpperChunk) {
    std::array<int32_t, Chunk::kWidth> height_map;
    for (int i = 0; i < Chunk::kWidth; ++i) {
      height_map[i] = utils::MapRange(
          noise1d_(chunk_pos.x() * Chunk::kWidth + i), PerlinNoise1D::kMin,
          PerlinNoise1D::kMax, 0, Chunk::kHeight - 1);
    }
    for (int32_t y = 0; y < Chunk::kHeight; ++y) {
      for (int32_t x = 0; x < Chunk::kWidth; ++x) {
        if (height_map[x] < y) {
          chunk.SetBlock(QPoint(x, y), Block(Block::Type::kDirt));
        } else if (height_map[x] == y) {
          chunk.SetBlock(QPoint(x, y), Block(Block::Type::kGrass));
        }
      }
    }
  }
  return chunk;
}

void PerlinChunkMapGenerator::PerlinRegionGenerator::GenerateCaves(
    Chunk* chunk, QPoint chunk_pos) {
  if (chunk_pos.y() < 1) {
    return;
  }
  for (int y = 0; y < Chunk::kHeight; ++y) {
    for (int x = 0; x < Chunk::kWidth; ++x) {
      if (noise2d_(kCavesScale * (chunk_pos.x() * Chunk::kWidth + x),
                   kCavesScale * (chunk_pos.y() * Chunk::kHeight + y)) >
          kCavesRate) {
        chunk->SetBlock(QPoint(x, y), Block(Block::Type::kAir));
      }
    }
  }
}
