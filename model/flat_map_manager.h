#ifndef MODEL_FLAT_MAP_MANAGER_H_
#define MODEL_FLAT_MAP_MANAGER_H_

#include "model/abstract_map_manager.h"

class FlatMapGenerator : public AbstractMapManager {
 public:
  AbstractMap* GenerateMap(const QString& save_file) override;
};

#endif  // MODEL_FLAT_MAP_MANAGER_H_
