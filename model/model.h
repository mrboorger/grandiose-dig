#ifndef MODEL_MODEL_H_
#define MODEL_MODEL_H_

#include <memory>
#include <unordered_set>

#include "model/map.h"
#include "model/player.h"

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

  std::shared_ptr<const Player> GetPlayer() const { return player_; }
  void SetPlayer(std::shared_ptr<Player> player) { player_ = player; }

  void MoveObjects(const std::unordered_set<int>& pressed_keys) {
    player_->Move(pressed_keys);
  }

 private:
  Model() = default;

  std::shared_ptr<Map> map_;
  std::shared_ptr<Player> player_;
};

#endif  // MODEL_MODEL_H_
