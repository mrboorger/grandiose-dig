#ifndef MODEL_CHUNK_H_
#define MODEL_CHUNK_H_

#include <QPoint>
#include <cassert>
#include <cstdint>
#include <vector>

#include "model/block.h"

class Chunk {
 public:
  static constexpr int32_t kWidth = 64;
  static constexpr int32_t kHeight = 64;

  Chunk() : blocks_(kWidth * kHeight, Block(Block::Type::kAir)) {}

  const Block& GetBlock(QPoint pos) const {
    assert(0 <= pos.x() && pos.x() < kWidth);
    assert(0 <= pos.y() && pos.y() < kHeight);
    return blocks_[kWidth * pos.y() + pos.x()];
  }

  Block* GetChangeableBlock(QPoint pos) {
    assert(0 <= pos.x() && pos.x() < kWidth);
    assert(0 <= pos.y() && pos.y() < kHeight);
    return &blocks_[kWidth * pos.y() + pos.x()];
  }

  void SetBlock(QPoint pos, Block block) {
    assert(0 <= pos.x() && pos.x() < kWidth);
    assert(0 <= pos.y() && pos.y() < kHeight);
    blocks_[kWidth * pos.y() + pos.x()] = block;
  }

 private:
  std::vector<Block> blocks_;
};

#endif  // MODEL_CHUNK_H_
