#ifndef MODEL_MAP_H_
#define MODEL_MAP_H_

#include <vector>

#include "model/block.h"
#include "model/abstract_map.h"

class Map : AbstractMap {
  friend class AbstractMapGenerator;
  friend class FlatMapGenerator;

 public:
  const Block& GetBlock(int32_t x, int32_t y) override;
  void SetBlock(int32_t x, int32_t y, Block block) override;

 private:
  Map(int width, int height);

  std::vector<Block> blocks_;
  int width_;
  int height_;
};

#endif  // MODEL_MAP_H_
