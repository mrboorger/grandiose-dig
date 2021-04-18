#include "inventory_drawer.h"

#include <QString>
#include <QTextItem>
#include <utility>

namespace {

std::array<QString, InventoryItem::kTypesCount> kNames = {"", "dirt.png",
                                                          "grass.png"};
std::array<QImage, InventoryItem::kTypesCount> images;

}  // namespace

InventoryDrawer::InventoryDrawer(std::shared_ptr<const Inventory> inventory)
    : inventory_(std::move(inventory)) {
  LoadInventoryBackground();
}

void InventoryDrawer::DrawInventory(QPainter* painter) {
  painter->drawPixmap(QPoint(0, 0), inventory_background_);
  for (int i = 0; i < Inventory::kItemsInColumn; ++i) {
    for (int j = 0; j < Inventory::kItemsInRow; ++j) {
      const auto& item = (*inventory_)[i * Inventory::kItemsInRow + j];
      if (item.IsEmpty()) {
        continue;
      }
      DrawItemSprite(painter, QPoint(j, i), item.GetId());
      DrawItemCount(painter, QPoint(j, i),
                    (*inventory_)[i * Inventory::kItemsInRow + j].GetCount());
    }
  }
}

void InventoryDrawer::DrawItemSprite(QPainter* painter, QPoint pos, int id) {
  if (images[id].isNull()) {
    images[id].load(":/resources/textures/" + kNames[id]);
  }
  painter->save();
  painter->setOpacity(kItemsOpacity);
  painter->drawImage(
      QPoint(pos.x() * (kCellSize + kIndentSize) + kItemCorner.x(),
             pos.y() * (kCellSize + kIndentSize) + kItemCorner.y()),
      images[id]);
  painter->restore();
}

void InventoryDrawer::DrawItemCount(QPainter* painter, QPoint pos, int cnt) {
  painter->save();
  painter->setOpacity(kInscriptionOpacity);
  painter->setFont(QFont("Helvetica [Cronyx]", kFontSize));
  QRect text_rect = QRect(
      QPoint(pos.x() * (kCellSize + kIndentSize),
             pos.y() * (kCellSize + kIndentSize) + kItemSize + kItemCorner.y()),
      QPoint(pos.x() * (kCellSize + kIndentSize) + kCellSize,
             pos.y() * (kCellSize + kIndentSize) + kCellSize));
  painter->drawText(text_rect, Qt::AlignCenter, QString::number(cnt));
  painter->restore();
}

void InventoryDrawer::LoadInventoryBackground() {
  inventory_background_ =
      QPixmap((kCellSize + kIndentSize) * Inventory::kItemsInRow,
              (kCellSize + kIndentSize) * Inventory::kItemsInColumn);
  inventory_background_.fill(Qt::transparent);

  QPainter back_painter(&inventory_background_);
  back_painter.setOpacity(kBackgroundOpacity);
  QImage cell_png(":/resources/textures/inventory_cell.png");
  for (int i = 0; i < Inventory::kItemsInColumn; ++i) {
    for (int j = 0; j < Inventory::kItemsInRow; ++j) {
      back_painter.drawImage(
          QPoint(j * (kCellSize + kIndentSize), i * (kCellSize + kIndentSize)),
          cell_png);
    }
  }
}
