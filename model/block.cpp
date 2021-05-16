#include "model/block.h"

#include <algorithm>

bool Block::DecreaseDurability(int delta) {
  if (type_ != Type::kAir) {
    durability_ = std::max(0.0, durability_ - delta);
  }
  return durability_ <= kDurEps;
}
