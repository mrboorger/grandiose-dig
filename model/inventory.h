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

  int GetSelectedColumn() const { return selected_column_; }
  int GetSelectedRow() const { return selected_row_; }
  const InventoryItem& GetSelectedItem() const;

  void ChangeSelectedColumn(int col) { selected_column_ = col; }

  bool CanCraft(const CraftRecipe& recipe);
  void Craft(const CraftRecipe& recipe);

  void SwitchToPrevRow();
  void SwitchToNextRow();

 private:
  static constexpr int kInventorySize = kItemsInColumn * kItemsInRow;

  std::array<InventoryItem, kInventorySize> items_;
  int selected_column_ = 0;
  int selected_row_ = 0;
};

#endif  // MODEL_INVENTORY_H_
