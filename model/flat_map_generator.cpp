#include "flat_map_generator.h"

Map FlatMapGenerator::GenerateMap() {
  // TODO(Wind-Eagle): delete
  Map map(300, 300);
  for (int y = 150; y < 300; ++y) {
    for (int x = 0; x < 300; ++x) {
      map.SetBlock(x, y, Block(Block::Type::kDirt));
    }
  }
  int32_t random = 123456;
  for (int y = 0; y < 150; ++y) {
    for (int x = 0; x < 300; ++x) {
      random ^= (random << 8);
      random ^= (random >> 11);
      random ^= (random << 9);
      if (random % 10 == 0) map.SetBlock(x, y, Block(Block::Type::kDirt));
    }
  }
  return map;
}
