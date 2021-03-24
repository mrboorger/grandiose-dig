#include "view/map_drawer.h"

#include <utility>

#include "model/constants.h"
#include "view/block_drawer.h"

MapDrawer::MapDrawer(std::shared_ptr<AbstractMap> map) : map_(std::move(map)) {}

void MapDrawer::DrawMapWithCenter(QPainter* painter, const QPointF& pos,
                                  const QRect& screen_coords) {
  for (int32_t y = pos.y() - kFieldOfView; y <= pos.y() + kFieldOfView; ++y) {
    for (int32_t x = pos.x() - kFieldOfView; x <= pos.x() + kFieldOfView; ++x) {
      QPointF point = (QPointF(x, y) - pos) * constants::kBlockSz +
                      screen_coords.center();

      BlockDrawer::DrawBlock(painter, point, map_->GetBlock(x, y));
    }
  }
}
