#include "model/perlin_chunk_map_manager.h"

#include <QCborMap>
#include <QCborValue>
#include <QFile>
#include <QJsonDocument>
#include <array>

#include "model/perlin_noise1d.h"
#include "utils.h"

AbstractMap* PerlinChunkMapManager::GenerateMap(const QString& save_file) {
  return new ChunkMap(save_file, new PerlinRegionGenerator(seed_));
}

PerlinChunkMapManager::PerlinChunkMapManager(uint32_t seed) : seed_(seed) {}

PerlinChunkMapManager::PerlinRegionGenerator::PerlinRegionGenerator(
    uint32_t seed)
    : noise1d_(seed), noise2d_(seed) {}

Chunk PerlinChunkMapManager::PerlinRegionGenerator::Generate(
    const QString& save_file, QPoint chunk_pos) {
  Chunk chunk;
  QFile file(save_file + "chunk:" + QString::number(chunk_pos.x()) + ":" +
             QString::number(chunk_pos.y()));
  if (!file.open(QIODevice::ReadOnly)) {
    chunk = BasicGeneration(chunk_pos);
    GenerateCaves(&chunk, chunk_pos);
    return chunk;
  }
  QByteArray save_data = file.readAll();
  QJsonDocument data(
      QJsonDocument(QCborValue::fromCbor(save_data).toMap().toJsonObject()));
  chunk.Read(data.object());
  return chunk;
}

Chunk PerlinChunkMapManager::PerlinRegionGenerator::BasicGeneration(
    QPoint chunk_pos) {
  Chunk chunk;
  if (chunk_pos.y() > kUpperChunk) {
    chunk.FillWith(Block(Block::FrontType::kDirt));
  } else if (chunk_pos.y() == kUpperChunk) {
    std::array<int32_t, Chunk::kWidth> height_map;
    for (int i = 0; i < Chunk::kWidth; ++i) {
      height_map[i] =
          utils::MapRange(noise1d_(chunk_pos.x() + i), PerlinNoise1D::kMin,
                          PerlinNoise1D::kMax, 0, Chunk::kHeight - 1);
    }
    for (int32_t y = 0; y < Chunk::kHeight; ++y) {
      for (int32_t x = 0; x < Chunk::kWidth; ++x) {
        if (height_map[x] < y) {
          chunk.SetBlock(QPoint(x, y), Block(Block::FrontType::kDirt));
        } else if (height_map[x] == y) {
          chunk.SetBlock(QPoint(x, y), Block(Block::FrontType::kGrass));
        }
      }
    }
  }
  return chunk;
}

void PerlinChunkMapManager::PerlinRegionGenerator::GenerateCaves(
    Chunk* chunk, QPoint chunk_pos) {
  if (chunk_pos.y() < kUpperChunk) {
    return;
  }
  for (int y = 0; y < Chunk::kHeight; ++y) {
    for (int x = 0; x < Chunk::kWidth; ++x) {
      if (noise2d_(kCavesScale * (chunk_pos.x() + x),
                   kCavesScale * (chunk_pos.y() + y)) > kCavesRate) {
        chunk->SetBlock(QPoint(x, y), Block(Block::FrontType::kAir));
      }
    }
  }
}
