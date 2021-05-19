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
  double biome_determination = noise1d_(chunk_pos.x() + chunk_pos.y());
  Chunk chunk = (biome_determination <= 0
                     ? LandscapeGeneration(chunk_pos, Biome::kHills)
                     : LandscapeGeneration(chunk_pos, Biome::kPlains));
  GenerateCaves(&chunk, chunk_pos);
  return chunk;
}

Chunk PerlinChunkMapGenerator::PerlinRegionGenerator::LandscapeGeneration(
    QPoint chunk_pos, Biome biome) {
  Chunk chunk;
  if (chunk_pos.y() > kUpperChunk) {
    chunk.FillWith(Block(Block::Type::kStone));
  } else if (chunk_pos.y() == kUpperChunk) {
    std::array<int32_t, Chunk::kWidth> height_map;
    double hill_height =
        (noise1d_(chunk_pos.x() + chunk_pos.y()) + 1) * Chunk::kHeight / 2;
    double left_hill_bound =
        (noise1d_(chunk_pos.x() + chunk_pos.y() * 2) + 1) * Chunk::kWidth / 2;
    double right_hill_bound =
        (noise1d_(chunk_pos.x() + chunk_pos.y() * 3) + 1) * Chunk::kWidth / 2;
    if (left_hill_bound > right_hill_bound) {
      std::swap(left_hill_bound, right_hill_bound);
    }
    for (int i = 0; i < Chunk::kWidth; ++i) {
      double range;
      switch (biome) {
        case Biome::kHills: {
          range = hill_height;
          if (i <= left_hill_bound) {
            range = std::min(range, hill_height * (1.0 - (left_hill_bound - i) /
                                                             left_hill_bound));
          } else if (i >= right_hill_bound) {
            range = std::min(
                range, hill_height * ((Chunk::kWidth - i) /
                                      (Chunk::kWidth - right_hill_bound)));
          }
          break;
        }
        case Biome::kPlains: {
          range = std::min({Chunk::kHeight / 4, i, Chunk::kWidth - i - 1});
          break;
        }
        default: {
          assert(false);
        }
      }
      height_map[i] =
          utils::MapRange(noise1d_(chunk_pos.x() + i), PerlinNoise1D::kMin,
                          PerlinNoise1D::kMax, 0, range);
    }

    for (int32_t x = 0; x < Chunk::kWidth; ++x) {
      std::normal_distribution<double> distrib(5.5, 0.25);
      int stone_height = distrib(utils::random);
      for (int32_t y = 0; y < Chunk::kHeight; ++y) {
        if (height_map[x] < y) {
          if (y - height_map[x] >= stone_height) {
            chunk.SetBlock(QPoint(x, y), Block(Block::Type::kStone));
          } else {
            chunk.SetBlock(QPoint(x, y), Block(Block::Type::kDirt));
          }
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
  if (chunk_pos.y() < kUpperChunk) {
    return;
  }
  for (int x = 0; x < Chunk::kWidth; ++x) {
    for (int y = 0; y < Chunk::kHeight; ++y) {
      if (noise2d_(kCavesScale * (chunk_pos.x() + x),
                   kCavesScale * (chunk_pos.y() + y)) > kCavesRate) {
        chunk->SetBlock(QPoint(x, y), Block(Block::Type::kAir));
      }
    }
  }
}
