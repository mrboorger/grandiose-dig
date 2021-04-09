#ifndef VIEW_INVENTORY_DRAWER_H_
#define VIEW_INVENTORY_DRAWER_H_

#include <QPainter>

#include "model/inventory.h"

class InventoryDrawer {
 public:
  explicit InventoryDrawer(std::shared_ptr<const Inventory> inventory);

  void DrawInventory(QPainter* painter);

 private:
  static constexpr float kBackgroundOpacity = 0.55;
  static constexpr float kItemsOpacity = 0.85;
  static constexpr float kInscriptionOpacity = 0.95;

  static constexpr int kItemSize = 16;  // in pixels
  static constexpr int kCellSize = 32;  // in pixels
  static constexpr int kIndentSize = 4;  // in pixels
  static constexpr int kFontSize = 8;

  static constexpr QPoint kItemCorner{8, 4};

  void LoadInventoryBackground();

  std::shared_ptr<const Inventory> inventory_;

  QPixmap inventory_background_;
};

#endif  // VIEW_INVENTORY_DRAWER_H_