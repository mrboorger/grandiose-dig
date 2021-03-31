#include "model/map.h"

#include <algorithm>
#include <utility>

const Block& Map::GetBlock(QPoint pos) {
  return blocks_[pos.y() * width_ + pos.x()];
}

void Map::SetBlock(QPoint pos, Block block) {
  blocks_[pos.y() * width_ + pos.x()] = block;
}

Map::Map(int width, int height)
    : blocks_(width * height, Block(Block::Type::kAir)),
      width_(width),
      height_(height) {}
