#include "model/perlin_chunk_map_generator.h"

#include <array>
#include <cmath>

#include "model/perlin_noise1d.h"
#include "utils.h"

AbstractMap* PerlinChunkMapGenerator::GenerateMap() {
  return new ChunkMap(new PerlinRegionGenerator(seed_));
}

PerlinChunkMapGenerator::PerlinChunkMapGenerator(uint32_t seed) : seed_(seed) {}

PerlinChunkMapGenerator::PerlinRegionGenerator::PerlinRegionGenerator(
    uint32_t seed)
    : noise_hills_(seed),
      noise_stone_(seed + 1),
      noise_caves_(seed + 2),
      noise_coal_(seed + 3),
      noise_coal2_(seed + 4),
      noise_iron_(seed + 5),
      noise_iron2_(seed + 6) {}

Chunk PerlinChunkMapGenerator::PerlinRegionGenerator::Generate(
    QPoint chunk_pos) {
  Chunk chunk = LandscapeGeneration(chunk_pos);
  GenerateOres(&chunk, chunk_pos);
  GenerateCaves(&chunk, chunk_pos);
  return chunk;
}

double PerlinChunkMapGenerator::PerlinRegionGenerator::HeightNoise(
    double noise) {
  return (1.0 / (1 + exp(-noise * kHillsRapidness))) * 2 - 1;
}

double PerlinChunkMapGenerator::PerlinRegionGenerator::StoneNoise(
    double noise) {
  return (1.0 / (1 + exp(-noise * kStoneRapidness))) * kStoneMaxHeight;
}

Chunk PerlinChunkMapGenerator::PerlinRegionGenerator::LandscapeGeneration(
    QPoint chunk_pos) {
  Chunk chunk;
  if (chunk_pos.y() > kUpperChunk) {
    chunk.FillWith(Block(Block::Type::kStone));
  } else if (chunk_pos.y() == kUpperChunk) {
    std::array<int32_t, Chunk::kWidth> height_map;
    for (int i = 0; i < Chunk::kWidth; ++i) {
      height_map[i] = utils::MapRange(
          HeightNoise(noise_hills_(chunk_pos.x() + i)), PerlinNoise1D::kMin,
          PerlinNoise1D::kMax, 0, Chunk::kHeight - 1);
    }
    for (int32_t x = 0; x < Chunk::kWidth; ++x) {
      int stone_height = StoneNoise(noise_stone_(chunk_pos.x() + x));
      for (int32_t y = 0; y < Chunk::kHeight; ++y) {
        if (height_map[x] < y) {
          if (y - height_map[x] <= stone_height) {
            chunk.SetBlock(QPoint(x, y), Block(Block::Type::kDirt));
          } else {
            chunk.SetBlock(QPoint(x, y), Block(Block::Type::kStone));
          }
        } else if (height_map[x] == y) {
          chunk.SetBlock(QPoint(x, y), Block(Block::Type::kGrass));
        }
      }
    }
  }
  return chunk;
}

void PerlinChunkMapGenerator::PerlinRegionGenerator::GenerateOres(
    Chunk* chunk, QPoint chunk_pos) {
  if (chunk_pos.y() < kUpperChunk) {
    return;
  }
  for (int y = 0; y < Chunk::kHeight; ++y) {
    for (int x = 0; x < Chunk::kWidth; ++x) {
      if (chunk->GetBlock(QPoint(x, y)).GetType() == Block::Type::kStone) {
        double coal = noise_coal_(kCoalScale * (chunk_pos.x() + x),
                                  kCoalScale * (chunk_pos.y() + y));
        if (coal > kCoalMainRate) {
          double coal2 = noise_coal2_(kCoalScale * (chunk_pos.x() + x),
                                      kCoalScale * (chunk_pos.y() + y));
          if (coal2 > kCoalRate) {
            chunk->SetBlock(QPoint(x, y), Block(Block::Type::kCoalOre));
          }
        }
      }
      if (chunk->GetBlock(QPoint(x, y)).GetType() == Block::Type::kStone) {
        double coal = noise_iron_(kIronScale * (chunk_pos.x() + x),
                                  kIronScale * (chunk_pos.y() + y));
        if (coal > kIronMainRate) {
          double coal2 = noise_iron2_(kIronScale * (chunk_pos.x() + x),
                                      kIronScale * (chunk_pos.y() + y));
          if (coal2 > kIronRate) {
            chunk->SetBlock(QPoint(x, y), Block(Block::Type::kIronOre));
          }
        }
      }
    }
  }
}

void PerlinChunkMapGenerator::PerlinRegionGenerator::GenerateCaves(
    Chunk* chunk, QPoint chunk_pos) {
  if (chunk_pos.y() < kUpperChunk) {
    return;
  }
  for (int y = 0; y < Chunk::kHeight; ++y) {
    for (int x = 0; x < Chunk::kWidth; ++x) {
      if (noise_caves_(kCavesScale * (chunk_pos.x() + x),
                       kCavesScale * (chunk_pos.y() + y)) > kCavesRate) {
        chunk->SetBlock(QPoint(x, y), Block(Block::Type::kAir));
      }
    }
  }
}
