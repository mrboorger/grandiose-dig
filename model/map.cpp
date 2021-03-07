#include "map.h"

#include <utility>

Map::Map(const Map& map)
    : blocks_(map.blocks_), kWidth(map.kWidth), kHeight(map.kHeight) {}

Map::Map(Map&& map) : kWidth(map.kWidth), kHeight(map.kHeight) {
  blocks_ = std::move(map.blocks_);
}

const Block& Map::GetBlock(int x, int y) const {
  return blocks_[y * kWidth + x];
}

void Map::SetBlock(int x, int y, Block block) {
  blocks_[y * kWidth + x] = block;
}

Map::Map(int width, int height)
    : blocks_(width * height, Block(Block::Type::kAir)),
      kWidth(width),
      kHeight(height) {}
