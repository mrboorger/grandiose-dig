#ifndef MODEL_MODEL_H_
#define MODEL_MODEL_H_

#include <memory>

#include "map.h"

class Model {
 public:
  static Model* GetInstance();

  Model(const Model&) = delete;
  Model(Model&&) = delete;

  std::shared_ptr<const Map> GetMap() const;
  void SetMap(std::shared_ptr<Map> map);

 private:
  Model() = default;

  std::shared_ptr<Map> map_;
};

#endif  // MODEL_MODEL_H_
