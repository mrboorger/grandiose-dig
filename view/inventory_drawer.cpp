#include "inventory_drawer.h"

#include <QString>
#include <QTextItem>
#include <utility>

namespace {

std::array<QImage, InventoryItem::kTypesCount> images;
std::array<QString, InventoryItem::kTypesCount> names = {"", "grass.png"};

}  // namespace

InventoryDrawer::InventoryDrawer(std::shared_ptr<const Inventory> inventory)
    : inventory_(std::move(inventory)) {
  inventory_background_ =
      QPixmap((kCellSize + kIndentSize) * inventory_->kItemsInRow,
              (kCellSize + kIndentSize) * inventory_->kItemsInColumn);
  inventory_background_.fill(Qt::transparent);
  LoadInventoryBackground();
}

void InventoryDrawer::DrawInventory(QPainter* painter) {
  painter->drawPixmap(QPoint(0, 0), inventory_background_);
  for (int i = 0; i < inventory_->kItemsInColumn; ++i) {
    for (int j = 0; j < inventory_->kItemsInRow; ++j) {
      int id = (*inventory_)[i * inventory_->kItemsInRow + j].GetId();
      if (id == 0) {
        continue;
      }
      DrawItemSprite(painter, QPoint(i, j), id);
      DrawItemCount(painter, QPoint(i, j),
                    (*inventory_)[i * inventory_->kItemsInRow + j].GetCount());
    }
  }
}

void InventoryDrawer::DrawItemSprite(QPainter* painter, QPoint pos, int id) {
  if (images[id].isNull()) {
    images[id].load(":/resources/textures" + names[id]);
  }
  painter->save();
  painter->setOpacity(kItemsOpacity);
  painter->drawImage(
      QPoint(pos.y() * (kCellSize + kIndentSize) + kItemCorner.x(),
             pos.x() * (kCellSize + kIndentSize) + kItemCorner.y()),
      QImage(":/resources/textures/" + names[id]));
  painter->restore();
}

void InventoryDrawer::DrawItemCount(QPainter* painter, QPoint pos, int cnt) {
  painter->save();
  painter->setOpacity(kInscriptionOpacity);
  painter->setFont(QFont("Helvetica [Cronyx]", kFontSize));
  QRect text_rect = QRect(
      QPoint(pos.y() * (kCellSize + kIndentSize),
             pos.x() * (kCellSize + kIndentSize) + kItemSize + kItemCorner.y()),
      QPoint(pos.y() * (kCellSize + kIndentSize) + kCellSize,
             pos.x() * (kCellSize + kIndentSize) + kCellSize));
  painter->drawText(text_rect, Qt::AlignCenter, QString::number(cnt));
  painter->restore();
}

void InventoryDrawer::LoadInventoryBackground() {
  inventory_background_ =
      QPixmap((kCellSize + kIndentSize) * inventory_->kItemsInRow,
              (kCellSize + kIndentSize) * inventory_->kItemsInColumn);
  inventory_background_.fill(Qt::transparent);

  QPainter back_painter(&inventory_background_);
  back_painter.setOpacity(kBackgroundOpacity);
  for (int i = 0; i < inventory_->kItemsInColumn; ++i) {
    for (int j = 0; j < inventory_->kItemsInRow; ++j) {
      back_painter.drawImage(
          QPoint(j * (kCellSize + kIndentSize), i * (kCellSize + kIndentSize)),
          QImage(":/resources/textures/inventory_cell.png"));
    }
  }
}
