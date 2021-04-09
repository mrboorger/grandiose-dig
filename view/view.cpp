#include "view/view.h"

#include <QPainter>

#include "controller/controller.h"
#include "model/constants.h"
#include "view/block_drawer.h"

View* View::GetInstance() {
  static View view;
  return &view;
}

View::View() : QWidget(nullptr), camera_(QPointF(150, 150)), drawer_(nullptr) {}

void View::SetInventoryDrawer(InventoryDrawer* drawer) {
  inventory_drawer_.reset(drawer);
}

void View::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);
  QPainter painter(this);
  camera_.SetPoint(Model::GetInstance()->GetPlayer()->GetPosition());
  drawer_->DrawMapWithCenter(&painter, camera_.GetPoint(), rect());
  inventory_drawer_->DrawInventory(&painter);

  // TODO(Wind-Eagle): temporary code; need to make PlayerDrawer
  auto player = Model::GetInstance()->GetPlayer();
  QImage player_image(":/resources/textures/player.png");
  QPointF point =
      (player->GetPosition() - camera_.GetPoint()) * constants::kBlockSz +
          rect().center();
  painter.drawImage(point, player_image);
}

void View::keyPressEvent(QKeyEvent* event) {
  Controller::GetInstance()->KeyPress(event->key());
}

void View::keyReleaseEvent(QKeyEvent* event) {
  Controller::GetInstance()->KeyRelease(event->key());
}
