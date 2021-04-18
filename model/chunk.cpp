#include "model/chunk.h"

void Chunk::FillWith(Block block) {
  for (int y = 0; y < kHeight; ++y) {
    for (int x = 0; x < kWidth; ++x) {
      SetBlock(QPoint(x, y), block);
    }
  }
}
