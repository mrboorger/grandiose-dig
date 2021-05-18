#include "view/buffered_map_drawer.h"

#include <cmath>
#include <memory>
#include <utility>

#include "model/constants.h"
#include "view/block_drawer.h"

BufferedMapDrawer::BufferedMapDrawer(std::shared_ptr<AbstractMap> map)
    : buffers_(constants::kDefaultClearTimeMSec), map_(std::move(map)) {}

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

void BufferedMapDrawer::UpdateBlock(QPoint pos) {
  auto buffer_pos = RoundToBufferPos(pos);
  auto buffer = buffers_.Get(buffer_pos);
  if (!buffer) {
    return;
  }
  QPixmap& casted = buffer.value();
  QPainter painter(&casted);
  RenderBlock(&painter,
              QPointF(pos.x() - buffer_pos.x(), pos.y() - buffer_pos.y()) *
                  constants::kBlockSz,
              map_->GetBlock(pos), true);
}

QRect BufferedMapDrawer::GetDrawRegion(QPoint center) const {
  return QRect(center.x() - kFieldOfView, center.y() - kFieldOfView,
               center.x() + kFieldOfView, center.y() + kFieldOfView);
}

QPoint BufferedMapDrawer::RoundToBufferPos(QPoint p) {
  return QPoint(p.x() - utils::ArithmeticalMod(p.x(), kBufferWidth),
                p.y() - utils::ArithmeticalMod(p.y(), kBufferHeight));
}

const QPixmap& BufferedMapDrawer::GetBufferPixmap(QPoint buffer_pos) {
  auto found = buffers_.Get(buffer_pos);
  if (!found) {
    auto& buffer = buffers_.Insert(
        buffer_pos, QPixmap(kPixmapWidthInPixels, kPixmapHeightInPixels));
    RenderBuffer(&buffer, buffer_pos);
    return buffer;
  }
  return found.value();
}

void BufferedMapDrawer::RenderBlock(QPainter* painter, QPointF block_drawer_pos,
                                    Block block, bool need_reset) {
  if (block.IsVisible()) {
    BlockDrawer::DrawBlock(painter, block_drawer_pos, block);
  } else if (need_reset) {
    BlockDrawer::ClearBlock(painter, block_drawer_pos);
  }
}

void BufferedMapDrawer::RenderBuffer(QPixmap* buffer, QPoint buffer_pos) {
  buffer->fill(Qt::transparent);
  QPainter painter(buffer);
  for (int y = 0; y < kBufferHeight; ++y) {
    for (int x = 0; x < kBufferWidth; ++x) {
      RenderBlock(
          &painter, QPointF(x, y) * constants::kBlockSz,
          map_->GetBlock(QPoint(buffer_pos.x() + x, buffer_pos.y() + y)));
    }
  }
}
