#ifndef VIEW_INVENTORY_DRAWER_H_
#define VIEW_INVENTORY_DRAWER_H_

#include <QPainter>
#include <QScrollArea>
#include <memory>

#include "model/model.h"

class InventoryDrawer {
 public:
  explicit InventoryDrawer(std::shared_ptr<const Inventory> inventory);

  void DrawInventory(QPainter* painter);

  void SetCraftMenuVisible(bool visible) { craft_menu_->setVisible(visible); }

 private:
  static constexpr float kBackgroundOpacity = 0.70;
  static constexpr float kItemsOpacity = 0.85;
  static constexpr float kInscriptionOpacity = 0.95;

  static constexpr int kItemSize = 16;   // in pixels
  static constexpr int kCellSize = 32;   // in pixels
  static constexpr int kIndentSize = 4;  // in pixels
  static constexpr int kFontSize = 8;

  static constexpr QPoint kItemCorner{8, 4};  // in pixels

  static constexpr QRect kMenuRect{10, 200, 250, 150};

  static constexpr int kCraftsInRow = 6;

  void DrawItemSprite(QPainter* painter, QPoint pos, int id);
  void DrawItemCount(QPainter* painter, QPoint pos, int cnt);
  void DrawSelectionBox(QPainter* painter);

  void LoadInventoryBackground();

  void CreateCraftScrollArea();

  std::shared_ptr<const Inventory> inventory_;

  QScrollArea* craft_menu_;

  QPixmap inventory_background_;
};

#endif  // VIEW_INVENTORY_DRAWER_H_
