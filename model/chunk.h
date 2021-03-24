#ifndef MODEL_CHUNK_H_
#define MODEL_CHUNK_H_

#include <cassert>
#include <cstdint>
#include <vector>

#include "model/block.h"

class Chunk {
 public:
  static constexpr int32_t kWidth = 64;
  static constexpr int32_t kHeight = 64;

  Chunk(int32_t x_pos, int32_t y_pos);

  const Block& GetBlock(int32_t x, int32_t y) const {
    assert(0 <= x && x < kWidth);
    assert(0 <= y && y < kHeight);
    return blocks_[kWidth * y + x];
  }

  void SetBlock(int32_t x, int32_t y, Block block) {
    assert(0 <= x && x < kWidth);
    assert(0 <= y && y < kHeight);
    blocks_[kWidth * y + x] = block;
  }

  int32_t GetPosX() const { return x_pos_; }
  int32_t GetPosY() const { return y_pos_; }

 private:
  std::vector<Block> blocks_;
  int32_t x_pos_;
  int32_t y_pos_;
};

#endif  // MODEL_CHUNK_H_
