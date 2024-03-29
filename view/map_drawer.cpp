#include "view/map_drawer.h"

#include <utility>

#include "model/constants.h"
#include "view/block_drawer.h"

MapDrawer::MapDrawer(std::shared_ptr<AbstractMap> map) : map_(std::move(map)) {}

void MapDrawer::DrawMapWithCenterImpl(QPainter* painter, const QPointF& pos,
                                  const QRect& screen_coords) {
  for (int32_t y = pos.y() - kFieldOfView; y <= pos.y() + kFieldOfView; ++y) {
    for (int32_t x = pos.x() - kFieldOfView; x <= pos.x() + kFieldOfView; ++x) {
      QPointF point =
          (QPointF(x, y) - pos) * constants::kBlockSz + screen_coords.center();

      BlockDrawer::DrawBlockFront(painter, point, map_->GetBlock(QPoint(x, y)));
    }
  }
}

QRect MapDrawer::GetDrawRegionImpl(QPoint center) const {
  return QRect(center.x() - kFieldOfView, center.y() - kFieldOfView,
               center.x() + kFieldOfView, center.y() + kFieldOfView);
}
