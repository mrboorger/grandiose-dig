#include "model/perlin_chunk_map_manager.h"

#include <QJsonObject>
#include <QCborMap>
#include <array>
#include <cmath>

#include "model/perlin_noise1d.h"
#include "utils.h"

AbstractMap* PerlinChunkMapManager::GenerateMap(const QString& save_file) {
  return new ChunkMap(save_file, new PerlinRegionGenerator(seed_));
}

PerlinChunkMapManager::PerlinChunkMapManager(uint32_t seed) : seed_(seed) {}

PerlinChunkMapManager::PerlinRegionGenerator::PerlinRegionGenerator(
    uint32_t seed)
    : noise_biome_temperature_(seed + 1),
      noise_hills_(seed + 10),
      noise_stone_(seed + 2),
      noise_caves_(seed + 5),
      noise_coal_(seed + 3),
      noise_coal2_(seed + 4),
      noise_iron_(seed + 8),
      noise_iron2_(seed + 9),
      noise_shimond_(seed + 11),
      noise_shimond2_(seed + 12) {}

Chunk PerlinChunkMapManager::PerlinRegionGenerator::Generate(const QString& save_file,
    QPoint chunk_pos) {
  Chunk chunk;
  QFile file(save_file + "chunk:" + QString::number(chunk_pos.x()) + ":" +
      QString::number(chunk_pos.y()));
  if (!file.exists()) {
    chunk = LandscapeGeneration(chunk_pos);
    GenerateOres(&chunk, chunk_pos);
    GenerateCaves(&chunk, chunk_pos);
    GenerateUndergroundDecoration(&chunk, chunk_pos);
    return chunk;
  }
  file.open(QIODevice::ReadOnly);
  QByteArray save_data = file.readAll();
  QJsonDocument data(
      QJsonDocument(QCborValue::fromCbor(save_data).toMap().toJsonObject()));
  chunk.Read(data.object());
  return chunk;
}

double PerlinChunkMapManager::PerlinRegionGenerator::PerlinBiomeNoise(
    double seed) {
  return noise_biome_temperature_(seed / kBiomeLength);
}

double PerlinChunkMapManager::PerlinRegionGenerator::HeightNoise(
    double noise) {
  return (1.0 / (1 + exp(-noise * kHillsRapidness))) * 2 - 1;
}

double PerlinChunkMapManager::PerlinRegionGenerator::StoneNoise(
    double noise) {
  return (1.0 / (1 + exp(-noise * kStoneRapidness))) * kStoneMaxHeight;
}

PerlinChunkMapManager::PerlinRegionGenerator::Biome
PerlinChunkMapManager::PerlinRegionGenerator::GetBiome(double noise) {
  if (noise < kColdNoise) {
    return Biome::kIcePlains;
  } else if (noise > kHotNoise) {
    return Biome::kDesert;
  } else {
    return Biome::kPlains;
  }
}

void PerlinChunkMapManager::PerlinRegionGenerator::GenerateBasicBiome(
    QPoint chunk_pos, Chunk* chunk, int x,
    const std::array<int32_t, Chunk::kWidth>& height_map, Block upper,
    Block lower) {
  int stone_height = StoneNoise(noise_stone_(chunk_pos.x() + x));
  for (int32_t y = 0; y < Chunk::kHeight; ++y) {
    if (height_map[x] < y) {
      if (y - height_map[x] <= stone_height) {
        chunk->SetBlock(QPoint(x, y), lower);
      } else {
        chunk->SetBlock(QPoint(x, y), Block(Block::FrontType::kStone));
      }
    } else if (height_map[x] == y) {
      chunk->SetBlock(QPoint(x, y), upper);
    }
  }
}
void PerlinChunkMapManager::PerlinRegionGenerator::GenerateIcePlains(
    QPoint chunk_pos, Chunk* chunk, int x,
    const std::array<int32_t, Chunk::kWidth>& height_map) {
  GenerateBasicBiome(chunk_pos, chunk, x, height_map,
                     Block(Block::FrontType::kSnowyGrass),
                     Block(Block::FrontType::kDirt));
}
void PerlinChunkMapManager::PerlinRegionGenerator::GeneratePlains(
    QPoint chunk_pos, Chunk* chunk, int x,
    const std::array<int32_t, Chunk::kWidth>& height_map) {
  GenerateBasicBiome(chunk_pos, chunk, x, height_map,
                     Block(Block::FrontType::kGrass),
                     Block(Block::FrontType::kDirt));
}
void PerlinChunkMapManager::PerlinRegionGenerator::GenerateDesert(
    QPoint chunk_pos, Chunk* chunk, int x,
    const std::array<int32_t, Chunk::kWidth>& height_map) {
  GenerateBasicBiome(chunk_pos, chunk, x, height_map,
                     Block(Block::FrontType::kSand),
                     Block(Block::FrontType::kSandstone));
}

Chunk PerlinChunkMapManager::PerlinRegionGenerator::LandscapeGeneration(
    QPoint chunk_pos) {
  Chunk chunk;
  if (chunk_pos.y() > kUpperChunk) {
    chunk.FillWith(Block(Block::FrontType::kStone));
  } else if (chunk_pos.y() == kUpperChunk) {
    std::array<int32_t, Chunk::kWidth> height_map;
    for (int i = 0; i < Chunk::kWidth; ++i) {
      height_map[i] = utils::MapRange(
          HeightNoise(noise_hills_(chunk_pos.x() + i)), PerlinNoise1D::kMin,
          PerlinNoise1D::kMax, 0, Chunk::kHeight - 1);
    }
    for (int32_t x = 0; x < Chunk::kWidth; ++x) {
      Biome biome = GetBiome(PerlinBiomeNoise(chunk_pos.x() + x));
      switch (biome) {
        case PerlinChunkMapManager::PerlinRegionGenerator::Biome::
          kIcePlains: {
          GenerateIcePlains(chunk_pos, &chunk, x, height_map);
          break;
        }
        case PerlinChunkMapManager::PerlinRegionGenerator::Biome::kPlains: {
          GeneratePlains(chunk_pos, &chunk, x, height_map);
          break;
        }
        case PerlinChunkMapManager::PerlinRegionGenerator::Biome::kDesert: {
          GenerateDesert(chunk_pos, &chunk, x, height_map);
          break;
        }
        default: {
          break;
        }
      }
    }
  }
  return chunk;
}

void PerlinChunkMapManager::PerlinRegionGenerator::GenerateOres(
    Chunk* chunk, QPoint chunk_pos) {
  if (chunk_pos.y() < kUpperChunk) {
    return;
  }
  for (int y = 0; y < Chunk::kHeight; ++y) {
    for (int x = 0; x < Chunk::kWidth; ++x) {
      if (chunk->GetBlock(QPoint(x, y)).GetFrontType() ==
          Block::FrontType::kStone) {
        double coal = abs(noise_coal_(kCoalScale * (chunk_pos.x() + x),
                                      kCoalScale * (chunk_pos.y() + y)));
        if (coal > kCoalMainRate) {
          double coal2 = abs(noise_coal2_(kCoalScale * (chunk_pos.x() + x),
                                          kCoalScale * (chunk_pos.y() + y)));
          if (coal2 > kCoalRate) {
            chunk->SetBlock(QPoint(x, y), Block(Block::FrontType::kCoalOre));
          }
        }
      }
      if (chunk->GetBlock(QPoint(x, y)).GetFrontType() ==
          Block::FrontType::kStone) {
        double iron = abs(noise_iron_(kIronScale * (chunk_pos.x() + x),
                                      kIronScale * (chunk_pos.y() + y)));
        if (iron > kIronMainRate) {
          double iron2 = abs(noise_iron2_(kIronScale * (chunk_pos.x() + x),
                                          kIronScale * (chunk_pos.y() + y)));
          if (iron2 > kIronRate) {
            chunk->SetBlock(QPoint(x, y), Block(Block::FrontType::kIronOre));
          }
        }
      }
    }
  }
}

void PerlinChunkMapManager::PerlinRegionGenerator::GenerateCaves(
    Chunk* chunk, QPoint chunk_pos) {
  if (chunk_pos.y() < kUpperChunk) {
    return;
  }
  for (int y = 0; y < Chunk::kHeight; ++y) {
    for (int x = 0; x < Chunk::kWidth; ++x) {
      if (noise_caves_(kCavesScale * (chunk_pos.x() + x),
                       kCavesScale * (chunk_pos.y() + y)) > kCavesRate) {
        chunk->GetBlockMutable(QPoint(x, y))
            ->SetFrontType(Block::FrontType::kAir);
      }
    }
  }
}

namespace {
bool IsNearBlock(Chunk* chunk, int x, int y, Block::FrontType type) {
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      if (i * j != 0) {
        continue;
      }
      if (Chunk::IsPositionLegit(QPoint(x + i, y + j)) &&
          chunk->GetBlock(QPoint(x + i, y + j)).GetFrontType() == type) {
        return true;
      }
    }
  }
  return false;
}

}  // namespace

void PerlinChunkMapManager::PerlinRegionGenerator::
GenerateUndergroundDecoration(Chunk* chunk, QPoint chunk_pos) {
  for (int x = 0; x < Chunk::kWidth; ++x) {
    Biome biome = GetBiome(PerlinBiomeNoise(chunk_pos.x() + x));
    if (biome != Biome::kIcePlains && biome != Biome::kDesert) {
      continue;
    }
    Block::FrontType type =
        (biome == Biome::kIcePlains ? Block::FrontType::kShimondOre
                                    : Block::FrontType::kFiremondOre);
    for (int y = 0; y < Chunk::kHeight; ++y) {
      if (chunk->GetBlock(QPoint(x, y)).GetFrontType() ==
          Block::FrontType::kStone) {
        double shifiremond =
            abs(noise_shimond_(kShifiremondScale * (chunk_pos.x() + x),
                               kShifiremondScale * (chunk_pos.y() + y)));
        if (shifiremond > kShifiremondMainRate) {
          double shifiremond2 =
              abs(noise_shimond2_(kShifiremondScale * (chunk_pos.x() + x),
                                  kShifiremondScale * (chunk_pos.y() + y)));
          if (shifiremond2 > kShifiremondLowerRate &&
              shifiremond2 < kShifiremondUpperRate) {
            if (IsNearBlock(chunk, x, y, Block::FrontType::kAir)) {
              chunk->SetBlock(QPoint(x, y), Block(type));
            }
          }
        }
      }
    }
  }
}

