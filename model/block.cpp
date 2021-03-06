#include "block.h"

#include <array>

Block::Block(Block::Type type) : type_(type) {}

Block::Type Block::GetType() {
  return type_;
}
int32_t Block::GetId() {
  return static_cast<int32_t>(type_);
}

bool Block::IsVisible() const {
  return IsVisible(type_);
}

bool Block::IsVisible(Block::Type type) {
  return type != Type::kAir;
}


