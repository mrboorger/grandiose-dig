#include "inventory.h"

#include <algorithm>
#include <utility>

Inventory::Inventory() {
  // TODO(mrboorger): it is a temporary code
  items_[0] = InventoryItem(InventoryItem::Type::kBlockGrass, 999);
  items_[1] = InventoryItem(InventoryItem::Type::kBlockGrass, 998);
  items_[2] = InventoryItem(InventoryItem::Type::kBlockGrass, 9);
  items_[3] = InventoryItem(InventoryItem::Type::kBlockGrass, 1);
}

void Inventory::AddItem(InventoryItem item) {
  for (InventoryItem& inventory_item : items_) {
    if (item.IsEmpty()) {
      return;
    }
    if (inventory_item.GetType() == item.GetType()) {
      int count_items_to_add =
          std::min(inventory_item.HowManyMoreItemsCanPut(), item.GetCount());
      item.ChangeCount(item.GetCount() - count_items_to_add);
      inventory_item.ChangeCount(inventory_item.GetCount() +
                                 count_items_to_add);
    }
  }
  if (item.GetCount() == 0) {
    return;
  }
  for (InventoryItem& inventory_item : items_) {
    if (inventory_item.IsEmpty()) {
      inventory_item = std::move(item);
      return;
    }
  }
}
