#include "model/flat_map_generator.h"

#include "model/map.h"

AbstractMap* FlatMapGenerator::GenerateMap() {
  // TODO(Wind-Eagle): delete
  auto* map = new Map(300, 300);
  for (int y = 150; y < 300; ++y) {
    for (int x = 0; x < 300; ++x) {
      map->SetBlock(QPoint(x, y), Block(Block::FrontType::kDirt));
    }
  }
  // TODO(Wind-Eagle): make better map generation. This is temporary code!
  int32_t random = 123456;
  for (int y = 0; y < 150; ++y) {
    for (int x = 0; x < 300; ++x) {
      random ^= (random << 8);
      random ^= (random >> 11);
      random ^= (random << 9);
      if (random % 10 == 0) {
        map->SetBlock(QPoint(x, y), Block(Block::FrontType::kDirt));
      }
    }
  }
  return map;
}
