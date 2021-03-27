#ifndef MODEL_INVENTORY_H_
#define MODEL_INVENTORY_H_

#include <array>

#include "model/inventory_item.h"

class Inventory {
 public:
  void AddItem(InventoryItem item);

 private:
  static constexpr int kInventorySize = 60;

  std::array<InventoryItem, kInventorySize> items_;
};

#endif  // MODEL_INVENTORY_H_
