#include "view.h"

#include <QPainter>

#include "block_drawer.h"
#include "model/constants.h"

View* View::GetInstance() {
  static View view;
  return &view;
}

View::View() : QWidget(nullptr), camera_(QPointF(150, 150)) {}

void View::paintEvent(QPaintEvent* event) {
  //
  Q_UNUSED(event);
  QPainter painter(this);
  const Map* map = Model::GetInstance()->GetMap();
  for (int y = static_cast<int>(camera_.GetPoint().y() - 50);
       y <= camera_.GetPoint().y() + 50; ++y) {
    for (int x = static_cast<int>(camera_.GetPoint().x() - 50);
         x <= camera_.GetPoint().x() + 50; ++x) {
      QPointF point(x, y);
      point -= camera_.GetPoint();

      point.setX(point.x() * constants::kBlockSz + (size().width() - constants::kBlockSz) / 2.0);
      point.setY(point.y() * constants::kBlockSz + (size().height() - constants::kBlockSz) / 2.0);

      BlockDrawer::DrawBlock(&painter, point, map->GetBlock(x, y));
    }
  }
}
