#include "model/map.h"

#include <algorithm>
#include <utility>

void Map::SetBlockImpl(QPoint pos, Block block) {
  blocks_[pos.y() * width_ + pos.x()] = block;
}

Block* Map::GetBlockMutableImpl(QPoint pos) {
  return &blocks_[pos.y() * width_ + pos.x()];
}

Map::Map(int width, int height)
    : blocks_(width * height, Block(Block::FrontType::kAir)),
      width_(width),
      height_(height) {}
