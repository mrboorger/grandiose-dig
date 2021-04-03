#include "model/flat_chunk_map_generator.h"

AbstractMap* FlatChunkMapGenerator::GenerateMap() {
  return new ChunkMap(new FlatRegionGenerator(kSeed));
}

FlatChunkMapGenerator::FlatRegionGenerator::FlatRegionGenerator(int seed)
    : gen_(seed) {}

Chunk FlatChunkMapGenerator::FlatRegionGenerator::Generate(QPoint chunk_pos) {
  Chunk chunk;
  if (chunk_pos.y() > 1) {
    for (int32_t y = 0; y < Chunk::kHeight; ++y) {
      for (int32_t x = 0; x < Chunk::kWidth; ++x) {
        chunk.SetBlock(QPoint(x, y), Block(Block::Type::kDirt));
      }
    }
  } else {
    for (int32_t y = 0; y < Chunk::kHeight; ++y) {
      for (int32_t x = 0; x < Chunk::kWidth; ++x) {
        if (gen_() % 20 == 0) {
          chunk.SetBlock(QPoint(x, y), Block(Block::Type::kDirt));
        }
      }
    }
  }
  return chunk;
}
