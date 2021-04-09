#ifndef MODEL_ABSTRACT_MAP_H_
#define MODEL_ABSTRACT_MAP_H_

#include "model/block.h"

class AbstractMap {
 public:
  AbstractMap() = default;
  AbstractMap(const AbstractMap&) = default;
  AbstractMap(AbstractMap&&) = default;

  virtual ~AbstractMap() = default;

  AbstractMap& operator=(const AbstractMap&) = default;
  AbstractMap& operator=(AbstractMap&&) = default;

  virtual const Block& GetBlock(int32_t x, int32_t y) = 0;
  virtual void SetBlock(int32_t x, int32_t y, Block block) = 0;
};

#endif  // MODEL_ABSTRACT_MAP_H_
