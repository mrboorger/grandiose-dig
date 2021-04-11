#include "block.h"

#include <iostream>

bool Block::DecreaseDurability(int delta) {
  if (type_ != Type::kAir) {
    durability_ -= delta;
  }
  std::cerr << durability_ << ' ' << delta << ' ' << int(type_) << std::endl;
  return durability_ <= 0;
}
