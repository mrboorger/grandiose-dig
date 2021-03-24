#ifndef MODEL_ABSTRACT_REGION_GENERATOR_H_
#define MODEL_ABSTRACT_REGION_GENERATOR_H_

#include <model/chunk.h>

class AbstractRegionGenerator {
 public:
  AbstractRegionGenerator() = default;
  AbstractRegionGenerator(const AbstractRegionGenerator&) = default;
  AbstractRegionGenerator(AbstractRegionGenerator&&) = default;

  virtual ~AbstractRegionGenerator() = default;

  AbstractRegionGenerator& operator=(const AbstractRegionGenerator&) = default;
  AbstractRegionGenerator& operator=(AbstractRegionGenerator&&) = default;

  virtual Chunk Generate(int32_t chunk_x, int32_t chunk_y) = 0;
};

#endif  // MODEL_ABSTRACT_REGION_GENERATOR_H_
