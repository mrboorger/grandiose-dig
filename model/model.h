#ifndef MODEL_MODEL_H_
#define MODEL_MODEL_H_

#include <memory>

#include "model/map.h"

class Model {
 public:
  static Model* GetInstance();

  Model(const Model&) = delete;
  Model(Model&&) = delete;

  ~Model() = default;

  Model& operator=(const Model&) = delete;
  Model& operator=(Model&&) = delete;

  std::shared_ptr<const Map> GetMap() const { return map_; }
  void SetMap(std::shared_ptr<Map> map) { map_ = map; }

 private:
  Model() = default;

  std::shared_ptr<Map> map_;
};

#endif  // MODEL_MODEL_H_
