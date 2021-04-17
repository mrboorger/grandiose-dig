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
    : noise_(seed) {}

Chunk PerlinChunkMapGenerator::PerlinRegionGenerator::Generate(
    QPoint chunk_pos) {
  Chunk chunk;
  if (chunk_pos.y() > 1) {
    chunk.FillWith(Block(Block::Type::kDirt));
  } else if (chunk_pos.y() == 1) {
    std::array<int32_t, Chunk::kWidth> height_map;
    for (int i = 0; i < Chunk::kWidth; ++i) {
      height_map[i] = utils::MapRange(noise_(chunk_pos.x() * Chunk::kWidth + i),
                                      PerlinNoise1D::kMin, PerlinNoise1D::kMax,
                                      0, Chunk::kHeight - 1);
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
