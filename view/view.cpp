#include "view.h"

#include <QPainter>

#include "model/constants.h"
#include "view/block_drawer.h"

View* View::GetInstance() {
  static View view;
  return &view;
}

View::View() : QWidget(nullptr), camera_(QPointF(150, 150)) {}

void View::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);
  QPainter painter(this);
  auto map = Model::GetInstance()->GetMap();
  for (int y = static_cast<int>(camera_.GetPoint().y() - kRenderDistance);
       y <= camera_.GetPoint().y() + kRenderDistance; ++y) {
    for (int x = static_cast<int>(camera_.GetPoint().x() - kRenderDistance);
         x <= camera_.GetPoint().x() + kRenderDistance; ++x) {
      QPointF point =
          (QPointF(x, y) - camera_.GetPoint()) * constants::kBlockSz +
          rect().center();

      BlockDrawer::DrawBlock(&painter, point, map->GetBlock(x, y));
    }
  }
}
