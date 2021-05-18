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

  std::shared_ptr<Inventory> GetInventory() { return inventory_; }

  void PickItem(InventoryItem item) { inventory_->AddItem(item); }

  void UseItem();

  bool IsBlockReachableForTool(QPoint block_coords);

  void SetAttackTick(int attack_tick) { attack_tick_ = attack_tick; }
  void SetAttackCooldownInterval(int attack_interval) {
    attack_cooldown_interval_ = attack_interval;
  }
  int GetAttackTick() const { return attack_tick_; }
  int GetAttackCooldownInterval() const { return attack_cooldown_interval_; }

  void SetAttackDirection(utils::Direction attack_direction) {
    attack_direction_ = attack_direction;
  }

  utils::Direction GetAttackDirection() const { return attack_direction_; }

  bool IsAttackDirectionLeft() const {
    return attack_direction_ == utils::Direction::kLeft;
  }

  void DecAttackTick(double time) {
    attack_tick_ = std::max(attack_tick_ - time, 0.0);
  }
  void DecAttackCooldownInterval(double time) {
    attack_cooldown_interval_ = std::max(attack_cooldown_interval_ - time, 0.0);
  }

  bool IsAttackFinished() const { return attack_tick_ <= constants::kEps; }
  bool CanStartAttack() const;

  // TODO(mrboorger):
  void SetUseItemCooldownInterval();
  void DecItemUsingCooldownInterval(double time);

  bool CanUseItem() const { return use_item_cooldown_interval == 0; }

 private:
  static constexpr int kUseItemCooldown = 100;

  std::shared_ptr<Inventory> inventory_;
  int attack_tick_ = 0;
  int attack_cooldown_interval_ = 0;
  utils::Direction attack_direction_ = utils::Direction::kLeft;

  int use_item_cooldown_interval = 0;

  // TODO(mrboorger): It is temporary
  int ToolRadius = 8;  // in blocks
};

#endif  // MODEL_PLAYER_H_
