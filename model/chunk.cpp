#include "model/chunk.h"

Chunk::Chunk(int32_t x_pos, int32_t y_pos)
    : blocks_(kWidth * kHeight, Block(Block::Type::kAir)),
      x_pos_(x_pos),
      y_pos_(y_pos) {}

