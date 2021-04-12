#include "view/buffered_map_drawer.h"

#include <cmath>
#include <memory>
#include <utility>

#include "view/block_drawer.h"

BufferedMapDrawer::BufferedMapDrawer(std::shared_ptr<AbstractMap> map)
    : map_(std::move(map)) {}

void BufferedMapDrawer::DrawMapWithCenter(QPainter* painter, const QPointF& pos,
                                       const QRect& screen_coords) {
  QPoint start = RoundToBufferPos(QPoint(pos.x(), pos.y()) -
                                  QPoint(kFieldOfView, kFieldOfView));
  QPoint finish = RoundToBufferPos(QPoint(pos.x(), pos.y()) +
                                   QPoint(kFieldOfView, kFieldOfView));
  map_->CacheRegion(QRect(start, finish));
  for (int32_t x = start.x(); x <= finish.x(); x += kBufferWidth) {
    for (int32_t y = start.y(); y <= finish.y(); y += kBufferHeight) {
      QPointF point =
          (QPointF(x, y) - pos) * constants::kBlockSz + screen_coords.center();
      painter->drawPixmap(point, GetBufferPixmap(QPoint(x, y)));
    }
  }
}

QPoint BufferedMapDrawer::RoundToBufferPos(QPoint p) {
  return QPoint(p.x() - utils::ArithmeticalMod(p.x(), kBufferWidth),
                p.y() - utils::ArithmeticalMod(p.y(), kBufferHeight));
}

const QPixmap& BufferedMapDrawer::GetBufferPixmap(QPoint buffer_pos) {
  if (buffers_.count(buffer_pos) == 0) {
    auto& node =
        buffers_
            .emplace(buffer_pos,
                     Buffer{QPixmap(kPixmapXInPixels, kPixmapYInPixels), true})
            .first->second;
    RenderBuffer(&node.pixmap, buffer_pos);
    return node.pixmap;
  }
  auto& node = buffers_.at(buffer_pos);
  node.is_used = true;
  return node.pixmap;
}

void BufferedMapDrawer::RenderBuffer(QPixmap* buffer, QPoint buffer_pos) {
  buffer->fill();
  QPainter painter(buffer);
  for (int y = 0; y < kBufferHeight; ++y) {
    for (int x = 0; x < kBufferWidth; ++x) {
      auto block =
          map_->GetBlock(QPoint(buffer_pos.x() + x, buffer_pos.y() + y));
      if (block.IsVisible()) {
        BlockDrawer::DrawBlock(&painter, QPointF(x, y) * constants::kBlockSz,
                               block);
      }
    }
  }
}
