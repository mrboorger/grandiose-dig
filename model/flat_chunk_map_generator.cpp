#include "model/flat_chunk_map_generator.h"

AbstractMap* FlatChunkMapGenerator::GenerateMap() {
  return new ChunkMap(new FlatRegionGenerator(kSeed));
}

FlatChunkMapGenerator::FlatRegionGenerator::FlatRegionGenerator(int seed)
    : gen_(seed) {}

Chunk FlatChunkMapGenerator::FlatRegionGenerator::Generate(int32_t chunk_x,
                                                           int32_t chunk_y) {
  Chunk chunk(chunk_x, chunk_y);
  if (chunk_y > 1) {
    for (int32_t y = 0; y < Chunk::kHeight; ++y) {
      for (int32_t x = 0; x < Chunk::kWidth; ++x) {
        chunk.SetBlock(x, y, Block(Block::Type::kDirt));
      }
    }
  } else {
    for (int32_t y = 0; y < Chunk::kHeight; ++y) {
      for (int32_t x = 0; x < Chunk::kWidth; ++x) {
        if (gen_() % 20 == 0) {
          chunk.SetBlock(x, y, Block(Block::Type::kDirt));
        }
      }
    }
  }
  return chunk;
}
