#include "flat_map_generator.h"

Map FlatMapGenerator::GenerateMap() {
  Map map(100, 100);
  for (int y = 0; y <= 50; ++y) {
    for (int x = 0; x < 100; ++x) {
      map.SetBlock(x, y, Block(Block::Type::kDirt));
    }
  }
  return map;
}
