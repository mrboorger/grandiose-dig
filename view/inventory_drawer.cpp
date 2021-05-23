#include "view/inventory_drawer.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QTextItem>
#include <utility>

#include "view.h"

namespace {

std::array<QString, InventoryItem::kTypesCount> kNames = {"", "dirt.png",
                                                          "grass.png"};
std::array<QImage, InventoryItem::kTypesCount> images;

QString selection_box_name = "selection_box.png";
QImage selection_box;

}  // namespace

InventoryDrawer::InventoryDrawer(std::shared_ptr<const Inventory> inventory)
    : inventory_(std::move(inventory)),
      craft_menu_(new QScrollArea(View::GetInstance())) {
  LoadInventoryBackground();
  CreateCraftScrollArea();
  craft_menu_->setStyleSheet("background-color: rgba(0,0,0, 100)");
}

void InventoryDrawer::DrawInventory(QPainter* painter) {
  painter->drawPixmap(QPoint(0, 0), inventory_background_);
  DrawSelectionBox(painter);
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
  painter->setFont(QFont("Calibri", kFontSize));
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

void InventoryDrawer::DrawSelectionBox(QPainter* painter) {
  if (selection_box.isNull()) {
    selection_box.load(":/resources/textures/" + selection_box_name);
  }
  assert(!selection_box.isNull());
  painter->drawImage(
      inventory_->GetSelectedItemNumber() * (kCellSize + kIndentSize), 0,
      selection_box);
}

void InventoryDrawer::CreateCraftScrollArea() {
  craft_menu_->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  craft_menu_->setGeometry(QRect(10, 200, 250, 150));
  craft_menu_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  craft_menu_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  craft_menu_->setVisible(false);

  QVBoxLayout* layout = new QVBoxLayout(craft_menu_);
  auto all_craft_recipes = Model::GetInstance()->GetAllCraftRecipes();
  for (int i = 0; i < all_craft_recipes->Size(); i += kCraftsInRow) {
    auto row_layout = new QHBoxLayout();
    for (int j = 0; j < kCraftsInRow && i + j < all_craft_recipes->Size();
         ++j) {
      const CraftRecipe& recipe = all_craft_recipes->GetRecipe(i + j);
      auto* button = new QPushButton;
      button->setFocusPolicy(Qt::FocusPolicy::NoFocus);
      button->setIcon(QIcon(":/resources/textures/" +
                            kNames[recipe.GetResultingItem().GetId()]));
      button->connect(button, &QPushButton::clicked, &recipe,
                      &CraftRecipe::TryCraft);
      row_layout->addWidget(button);
    }
    layout->addLayout(row_layout);
  }
  QWidget* widget = new QWidget;
  widget->setLayout(layout);
  craft_menu_->setWidget(widget);
}
