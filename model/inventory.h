#ifndef MODEL_INVENTORY_H_
#define MODEL_INVENTORY_H_

#include <array>

#include "model/inventory_item.h"

class Inventory {
 public:
  static constexpr int kItemsInColumn = 6;
  static constexpr int kItemsInRow = 10;

  Inventory();

  const InventoryItem& operator[](int ind) const { return items_[ind]; }
  void AddItem(InventoryItem item);

 private:
  static constexpr int kInventorySize = kItemsInColumn * kItemsInRow;

  std::array<InventoryItem, kInventorySize> items_;
};

#endif  // MODEL_INVENTORY_H_
