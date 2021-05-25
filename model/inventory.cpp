#include "inventory.h"

#include <QJsonArray>
#include <algorithm>
#include <map>
#include <utility>

Inventory::Inventory() {
  items_[0] = InventoryItem(InventoryItem::Type::kSpeedPotion, 3);
  items_[1] = InventoryItem(InventoryItem::Type::kSpeedPotion, 3);
}

void Inventory::AddItem(InventoryItem item) {
  for (InventoryItem& inventory_item : items_) {
    if (item.IsEmpty()) {
      return;
    }
    if (inventory_item.GetType() == item.GetType()) {
      int count_items_to_add =
          std::min(inventory_item.ItemsLeft(), item.GetCount());
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
  // We cannot add the item, so it disappears
}

void Inventory::RemoveOneSelectedItem() {
  int selected_item_num = selected_row_ * kItemsInRow + selected_column_;
  if (!items_[selected_item_num].IsEmpty()) {
    items_[selected_item_num].ChangeCount(items_[selected_item_num].GetCount() -
                                          1);
  }
}

const InventoryItem& Inventory::GetSelectedItem() const {
  return items_[selected_row_ * kItemsInRow + selected_column_];
}

bool Inventory::CanCraft(const CraftRecipe& recipe) {
  auto needed_items = recipe.GetNeededItems();
  for (const auto& item : items_) {
    auto it = std::find(needed_items.begin(), needed_items.end(), item);
    if (it != needed_items.end()) {
      it->ChangeCount(std::max(0, it->GetCount() - item.GetCount()));
    }
  }
  for (const auto& item : needed_items) {
    if (item.GetCount() != 0) {
      return false;
    }
  }
  return true;
}

void Inventory::Craft(const CraftRecipe& recipe) {
  auto needed_items = recipe.GetNeededItems();
  for (auto& item : items_) {
    auto it = std::find(needed_items.begin(), needed_items.end(), item);
    if (it != needed_items.end()) {
      int needed_count = it->GetCount();
      it->ChangeCount(std::max(0, needed_count - item.GetCount()));
      item.ChangeCount(std::max(0, item.GetCount() - needed_count));
    }
  }
  AddItem(recipe.GetResultingItem());
}

void Inventory::Read(const QJsonObject& json) {
  QJsonArray items = json["items"].toArray();
  for (int index = 0; index < kInventorySize; ++index) {
    items_[index].Read(items[index].toObject());
  }
}

void Inventory::Write(QJsonObject* json) const {
  QJsonArray items;
  for (int index = 0; index < kInventorySize; ++index) {
    QJsonObject item;
    items_[index].Write(&item);
    items.append(item);
  }
  (*json)["items"] = items;
}

void Inventory::SwitchToPrevRow() {
  --selected_row_;
  if (selected_row_ < 0) {
    selected_row_ = kItemsInColumn - 1;
  }
}

void Inventory::SwitchToNextRow() {
  ++selected_row_;
  if (selected_row_ == kItemsInColumn) {
    selected_row_ = 0;
  }
}
