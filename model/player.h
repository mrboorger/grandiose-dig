#ifndef MODEL_PLAYER_H_
#define MODEL_PLAYER_H_

#include <QPoint>
#include <memory>

#include "model/inventory.h"
#include "model/moving_object.h"

class Player : public MovingObject {
 public:
  explicit Player(QPointF pos);

  std::shared_ptr<const Inventory> GetInventory() const { return inventory_; }

  void PickItem(InventoryItem item) { inventory_->AddItem(item); }
  void SetAttackTick(int attack_tick) { attack_tick_ = attack_tick; }
  void SetAttackInterval(int attack_interval) {
    attack_interval_ = attack_interval;
  }
  int GetAttackTick() const { return attack_tick_; }
  int GetAttackInterval() const { return attack_interval_; }

  void SetAttackLeft(bool attack_left) { attack_left_ = attack_left; }

  bool IsAttackLeft() const { return attack_left_; }

  void DecAttackTick() { attack_tick_ = std::max(attack_tick_ - 1, 0); }
  void DecAttackInterval() {
    attack_interval_ = std::max(attack_interval_ - 1, 0);
  }

 private:
  std::shared_ptr<Inventory> inventory_;
  int attack_tick_ = 0;
  int attack_interval_ = 0;
  bool attack_left_ = true;
};

#endif  // MODEL_PLAYER_H_
