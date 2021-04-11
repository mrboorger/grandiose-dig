#ifndef MODEL_PLAYER_H_
#define MODEL_PLAYER_H_

#include <QPoint>
#include <memory>

#include "model/moving_object.h"
#include "model/inventory.h"

class Player : public MovingObject {
 public:
  explicit Player(QPointF pos);

  std::shared_ptr<const Inventory> GetInventory() const { return inventory_; }

  void PickItem(InventoryItem item) { inventory_->AddItem(item); }

 private:
  std::shared_ptr<Inventory> inventory_;
};

#endif  // MODEL_PLAYER_H_
