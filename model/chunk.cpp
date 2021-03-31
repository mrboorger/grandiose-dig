#include "model/chunk.h"

Chunk::Chunk(QPoint pos)
    : blocks_(kWidth * kHeight, Block(Block::Type::kAir)), pos_(pos) {}
