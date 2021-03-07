#ifndef MODEL_MAP_H_
#define MODEL_MAP_H_

#include <vector>

#include "block.h"

class Map {
  friend class AbstractMapGenerator;
  friend class FlatMapGenerator;

 public:
  explicit Map(const Map& map);
  Map(Map&& map);

  Map operator=(const Map& map) = delete;
  Map operator=(Map&& map) = delete;

  const Block& GetBlock(int x, int y) const;
  void SetBlock(int x, int y, Block block);

 private:
  Map(int width, int height);

  const int kWidth;
  const int kHeight;

  std::vector<Block> blocks_;
};

#endif  // MODEL_MAP_H_
