#include "model/map.h"

#include <algorithm>
#include <utility>

const Block& Map::GetBlock(int32_t x, int32_t y) {
  return blocks_[y * width_ + x];
}

void Map::SetBlock(int32_t x, int32_t y, Block block) {
  blocks_[y * width_ + x] = block;
}

Map::Map(int width, int height)
    : blocks_(width * height, Block(Block::Type::kAir)),
      width_(width),
      height_(height) {}
