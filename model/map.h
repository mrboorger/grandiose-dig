#ifndef MODEL_MAP_H_
#define MODEL_MAP_H_

#include <vector>

#include "model/block.h"

class Map {
  friend class AbstractMapGenerator;
  friend class FlatMapGenerator;

 public:
  Map(const Map& map);
  Map(Map&& map) noexcept;

  ~Map() = default;

  Map& operator=(const Map& rhs);
  Map& operator=(Map&& rhs) noexcept;

  void Swap(Map& other);

  const Block& GetBlock(int x, int y) const;
  void SetBlock(int x, int y, Block block);

 private:
  Map(int width, int height);

  std::vector<Block> blocks_;
  int width_;
  int height_;
};

#endif  // MODEL_MAP_H_
