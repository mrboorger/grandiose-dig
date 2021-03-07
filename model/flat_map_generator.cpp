#include "flat_map_generator.h"

Map FlatMapGenerator::GenerateMap() {
  // TODO: (Wind-Eagle: delete)
  Map map(300, 300);
  for (int y = 150; y < 300; ++y) {
    for (int x = 0; x < 300; ++x) {
      map.SetBlock(x, y, Block(Block::Type::kDirt));
    }
  }
  map.SetBlock(150, 150, Block(Block::Type::kAir));
  return map;
}
