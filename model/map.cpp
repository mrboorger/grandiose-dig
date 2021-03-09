#include "map.h"

#include <algorithm>
#include <utility>

const Block& Map::GetBlock(int x, int y) const {
  return blocks_[y * width_ + x];
}

void Map::SetBlock(int x, int y, Block block) {
  blocks_[y * width_ + x] = block;
}

Map::Map(int width, int height)
    : blocks_(width * height, Block(Block::Type::kAir)),
      width_(width),
      height_(height) {}
