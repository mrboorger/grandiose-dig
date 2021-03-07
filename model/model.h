#ifndef MODEL_MODEL_H_
#define MODEL_MODEL_H_

#include <memory>

#include "model/map.h"
#include "model/player.h"

class Model {
 public:
  static Model* GetInstance();

  Model(const Model&) = delete;
  Model(Model&&) = delete;

  std::shared_ptr<const Map> GetMap() const;
  void SetMap(std::shared_ptr<Map> map);

  std::shared_ptr<const Player> GetPlayer() const;
  void SetPlayer(std::shared_ptr<Player> player);

 private:
  Model() = default;

  std::shared_ptr<Map> map_;
  std::shared_ptr<Player> player_;
};

#endif  // MODEL_MODEL_H_
