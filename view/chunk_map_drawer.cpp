#include "view/chunk_map_drawer.h"

#include <QDebug>
#include <QFile>
#include <cmath>
#include <memory>
#include <utility>

#include "view/block_drawer.h"

ChunkMapDrawer::ChunkMapDrawer(std::shared_ptr<ChunkMap> map)
    : map_(std::move(map)), clear_timer_() {
  clear_timer_.callOnTimeout([this]() { ClearUnusedNodes(); });
  clear_timer_.start(kClearTimeMSec);
}

void ChunkMapDrawer::DrawMapWithCenter(QPainter* painter, const QPointF& pos,
                                       const QRect& screen_coords) {
  QPoint chunk_pos{0, 0};
  ChunkMap::GetChunkCoords(pos, &chunk_pos);
  for (int32_t curr_y = chunk_pos.y() - kFieldOfView;
       curr_y <= chunk_pos.y() + kFieldOfView; ++curr_y) {
    for (int32_t curr_x = chunk_pos.x() - kFieldOfView;
         curr_x <= chunk_pos.x() + kFieldOfView; ++curr_x) {
      QPointF point = (ChunkMap::GetWorldCoords(QPoint(curr_x, curr_y)) - pos) *
                          constants::kBlockSz +
                      screen_coords.center();
      painter->drawPixmap(point, GetChunkPixmap(QPoint(curr_x, curr_y)));
    }
  }
}

const QPixmap& ChunkMapDrawer::GetChunkPixmap(QPoint chunk_pos) {
  map_->UseChunk(chunk_pos);
  for (auto& [pixmap, pos, is_used] : render_buffer_) {
    if (pos == chunk_pos) {
      is_used = true;
      return pixmap;
    }
  }
  auto& node = render_buffer_.emplace_back(
      Node{QPixmap(kPixmapXInPixels, kPixmapYInPixels), chunk_pos, true});
  RenderChunk(&node.pixmap, map_->GetChunk(chunk_pos));
  return node.pixmap;
}

void ChunkMapDrawer::RenderChunk(QPixmap* buffer, const Chunk& chunk) {
  buffer->fill();
  QPainter painter(buffer);
  for (int y = 0; y < Chunk::kHeight; ++y) {
    for (int x = 0; x < Chunk::kWidth; ++x) {
      auto block = chunk.GetBlock(QPoint(x, y));
      if (block.IsVisible()) {
        BlockDrawer::DrawBlock(&painter, QPointF(x, y) * constants::kBlockSz,
                               chunk.GetBlock(QPoint(x, y)));
      }
    }
  }
}

void ChunkMapDrawer::ClearUnusedNodes() {
  for (int i = 0; i < static_cast<int>(render_buffer_.size()); ++i) {
    if (!render_buffer_[i].is_used) {
      std::swap(render_buffer_[i], render_buffer_.back());
      render_buffer_.pop_back();
      --i;
    } else {
      render_buffer_[i].is_used = false;
    }
  }
}

