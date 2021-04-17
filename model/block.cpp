#include "model/block.h"

bool Block::DecreaseDurability(int delta) {
  if (type_ != Type::kAir) {
    durability_ -= delta;
  }
  return durability_ <= 0;
}
