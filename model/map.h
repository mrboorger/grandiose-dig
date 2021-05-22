#ifndef MODEL_MAP_H_
#define MODEL_MAP_H_

#include <vector>

#include "model/abstract_map.h"
#include "model/block.h"

class Map : public AbstractMap {
  friend class AbstractMapGenerator;
  friend class FlatMapGenerator;

 private:
  Map(int width, int height);

  void SetBlockImpl(QPoint pos, Block block) override;


  Block* GetBlockMutable(QPoint pos) override;

  std::vector<Block> blocks_;
  int width_;
  int height_;
};

#endif  // MODEL_MAP_H_
