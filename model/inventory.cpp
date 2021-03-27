#include "inventory.h"

void Inventory::AddItem(InventoryItem item) {
  for (InventoryItem& inventory_item : items_) {
    if (item.GetCount() == 0) {
      return;
    }
    if (inventory_item.GetItemType() == item.GetItemType()) {
      int count_items_to_add =
          std::min(inventory_item.HowManyMoreItemsCanPut(), item.GetCount());
      item.ChangeCount(item.GetCount() - count_items_to_add);
      inventory_item.ChangeCount(
          inventory_item.GetCount() + count_items_to_add);
    }
  }
  if (item.GetCount() == 0) {
    return;
  }
  for (InventoryItem& inventory_item : items_) {
    if (inventory_item.GetItemType() == InventoryItem::ItemType::kEmptyItem) {
      inventory_item = item;
      return;
    }
  }
}
