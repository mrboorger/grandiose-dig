#include "inventory_drawer.h"

#include <QString>

#include <iostream>
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

void InventoryDrawer::LoadInventoryBackground() {
  inventory_background_ =
      QPixmap((kCellSize + kIndentSize) * inventory_->kItemsInRow,
              (kCellSize + kIndentSize) * inventory_->kItemsInColumn);
  inventory_background_.fill(Qt::transparent);

  QPainter back_painter(&inventory_background_);
  back_painter.setOpacity(kBackgroundOpacity);
  for (int i = 0; i < inventory_->kItemsInColumn; ++i) {
    for (int j = 0; j < inventory_->kItemsInRow; ++j) {
      back_painter.drawImage(QPoint(j * 36, i * 36),
                             QImage(":/resources/textures/inventory_cell.png"));
    }
  }
}

void InventoryDrawer::DrawInventory(QPainter* painter) {
  // TODO(mrboorger): it is temporary code

  painter->drawPixmap(QPoint(0, 0), inventory_background_);
  painter->save();

  painter->setOpacity(kItemsOpacity);
  for (int i = 0; i < inventory_->kItemsInColumn; ++i) {
    for (int j = 0; j < inventory_->kItemsInRow; ++j) {
      int id = (*inventory_)[i * inventory_->kItemsInRow + j].GetId();
      if (id == 0) {
        continue;
      }
      if (images[id].isNull()) {
        images[id].load(":/resources/textures" + names[id]);
      }
      painter->drawImage(QPoint(j * (kCellSize + kIndentSize) + kItemCorner.x(),
                                i * (kCellSize + kIndentSize)
                                    + kItemCorner.y()),
                         QImage(":/resources/textures/" + names[id]));

      painter->save();
      painter->setOpacity(kInscriptionOpacity);
      painter->setFont(QFont("Helvetica [Cronyx]", kFontSize));
      painter->drawText(QRect(QPoint(j * (kCellSize + kIndentSize),
                                     i * (kCellSize + kIndentSize) + kItemSize
                                         + kItemCorner.y()),
                              QPoint(j * (kCellSize + kIndentSize) + kCellSize,
                                     i * (kCellSize + kIndentSize)
                                         + kCellSize)),
                        Qt::AlignCenter,
                        QString::number((*inventory_)[
                                            i * inventory_->kItemsInRow
                                                + j].GetCount()));
      painter->restore();
    }
  }

  painter->restore();
}
