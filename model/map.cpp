#include "map.h"

#include <algorithm>
#include <utility>

Map::Map(const Map& map)
    : blocks_(map.blocks_), width_(map.width_), height_(map.height_) {}

Map::Map(Map&& map) noexcept : width_(map.width_), height_(map.height_) {
  blocks_ = std::move(map.blocks_);
}

Map& Map::operator=(const Map& rhs) {
  Map tmp(rhs);
  Swap(tmp);
  return *this;
}

Map& Map::operator=(Map&& rhs) noexcept {
  Map tmp(std::move(rhs));
  Swap(tmp);
  return *this;
}

void Map::Swap(Map& other) {
  blocks_.swap(other.blocks_);
  std::swap(width_, other.width_);
  std::swap(width_, other.height_);
}

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
