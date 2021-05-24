#include "model/map.h"

#include <QJsonArray>
#include <algorithm>
#include <utility>

void Map::SetBlock(QPoint pos, Block block) {
  blocks_[pos.y() * width_ + pos.x()] = block;
}

Block* Map::GetBlockMutable(QPoint pos) {
  return &blocks_[pos.y() * width_ + pos.x()];
}

Map::Map(int width, int height)
    : blocks_(width * height, Block(Block::Type::kAir)),
      width_(width),
      height_(height) {}
