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

  Block& operator[](int32_t i) { return blocks_[i]; }
  const Block& operator[](int32_t i) const { return blocks_[i]; }

  const Block& GetBlock(QPoint pos) const { return blocks_[BlockIndex(pos)]; }

  Block* GetBlockMutable(QPoint pos) { return &blocks_[BlockIndex(pos)]; }

  void SetBlock(QPoint pos, Block block) { blocks_[BlockIndex(pos)] = block; }

  void FillWith(Block block);

  void Read(const QJsonObject& json);
  void Write(QJsonObject* json) const;

 private:
  static int BlockIndex(QPoint pos) {
    assert(0 <= pos.x() && pos.x() < kWidth);
    assert(0 <= pos.y() && pos.y() < kHeight);
    return kWidth * pos.y() + pos.x();
  }

  std::vector<Block> blocks_;
};

#endif  // MODEL_CHUNK_H_
