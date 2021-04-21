#ifndef MODEL_PLAYER_H_
#define MODEL_PLAYER_H_

#include <QPoint>
#include <algorithm>
#include <memory>

#include "model/inventory.h"
#include "model/moving_object.h"
#include "utils.h"

class Player : public MovingObject {
 public:
  explicit Player(QPointF pos);

  std::shared_ptr<const Inventory> GetInventory() const { return inventory_; }

  void PickItem(InventoryItem item) { inventory_->AddItem(item); }
  void SetAttackTick(int attack_tick) { attack_tick_ = attack_tick; }
  void SetAttackCooldownInterval(int attack_interval) {
    attack_cooldown_interval_ = attack_interval;
  }
  int GetAttackTick() const { return attack_tick_; }
  int GetAttackCooldownInterval() const { return attack_cooldown_interval_; }

  void SetAttackDirection(utils::Direction attack_direction) {
    attack_direction_ = attack_direction;
  }

  bool IsAttackDirectionLeft() const {
    return attack_direction_ == utils::Direction::kLeft;
  }

  void DecAttackTick() { attack_tick_ = std::max(attack_tick_ - 1, 0); }
  void DecAttackCooldownInterval() {
    attack_cooldown_interval_ = std::max(attack_cooldown_interval_ - 1, 0);
  }

  bool IsAttackFinished() const { return attack_tick_ == 0; }
  bool CanStartAttack() const { return attack_cooldown_interval_ == 0; }

 private:
  std::shared_ptr<Inventory> inventory_;
  int attack_tick_ = 0;
  int attack_cooldown_interval_ = 0;
  utils::Direction attack_direction_ = utils::Direction::kLeft;
};

#endif  // MODEL_PLAYER_H_
