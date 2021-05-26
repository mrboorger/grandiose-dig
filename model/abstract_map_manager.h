#ifndef MODEL_ABSTRACT_MAP_MANAGER_H_
#define MODEL_ABSTRACT_MAP_MANAGER_H_

#include "model/abstract_map.h"

class AbstractMapManager {
 public:
  AbstractMapManager() = default;
  AbstractMapManager(const AbstractMapManager&) = default;
  AbstractMapManager(AbstractMapManager&&) = default;

  virtual ~AbstractMapManager() = default;

  AbstractMapManager& operator=(const AbstractMapManager&) = default;
  AbstractMapManager& operator=(AbstractMapManager&&) = default;

  virtual AbstractMap* GenerateMap(const QString& save_file) = 0;
};

#endif  // MODEL_ABSTRACT_MAP_MANAGER_H_
