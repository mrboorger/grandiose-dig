#ifndef MODEL_INVENTORY_H_
#define MODEL_INVENTORY_H_

#include <QJsonObject>
#include <array>

#include "model/inventory_item.h"

class Inventory {
 public:
  static constexpr int kItemsInColumn = 6;
  static constexpr int kItemsInRow = 10;

  Inventory();

  const InventoryItem& operator[](int ind) const { return items_[ind]; }
  void AddItem(InventoryItem item);

  void Read(const QJsonObject& json);
  void Write(QJsonObject& json) const;

 private:
  static constexpr int kInventorySize = kItemsInColumn * kItemsInRow;

  std::array<InventoryItem, kInventorySize> items_;
};

#endif  // MODEL_INVENTORY_H_
