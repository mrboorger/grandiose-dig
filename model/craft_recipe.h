#ifndef CRAFT_RECIPE_H_
#define CRAFT_RECIPE_H_

#include <QDebug>
#include <QObject>
#include <vector>

#include "model/inventory_item.h"

class CraftRecipe : public QObject {
  Q_OBJECT

 public:
  CraftRecipe(InventoryItem resulting_item,
              std::vector<InventoryItem> needed_items);
  CraftRecipe(CraftRecipe&& recipe) noexcept;
  virtual ~CraftRecipe() {}

  const std::vector<InventoryItem>& GetNeededItems() const {
    return needed_items_;
  }
  const InventoryItem& GetResultingItem() const { return resulting_item_; }

  void TryCraft() const;

 private:
  InventoryItem resulting_item_;
  std::vector<InventoryItem> needed_items_;
};

#endif  // CRAFT_RECIPE_H_
