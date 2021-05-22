#ifndef MODEL_INVENTORY_H_
#define MODEL_INVENTORY_H_

#include <array>

#include "model/craft_recipe.h"
#include "model/inventory_item.h"

class Inventory {
 public:
  static constexpr int kItemsInColumn = 5;
  static constexpr int kItemsInRow = 10;

  Inventory();

  const InventoryItem& operator[](int ind) const { return items_[ind]; }
  void AddItem(InventoryItem item);

  void RemoveOneSelectedItem();

  int GetSelectedItemNumber() const { return selected_item_; };
  const InventoryItem& GetSelectedItem() const;

  void ChangeSelectedItem(int selected_item) { selected_item_ = selected_item; }

  bool CanCraft(const CraftRecipe& recipe);
  void Craft(const CraftRecipe& recipe);

 private:
  static constexpr int kInventorySize = kItemsInColumn * kItemsInRow;

  std::array<InventoryItem, kInventorySize> items_;
  int selected_item_ = 0;  // 0-9
};

#endif  // MODEL_INVENTORY_H_
