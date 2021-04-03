#include "view/chunk_map_drawer.h"

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
  if (render_buffer_.count(chunk_pos) == 0) {
    auto& node =
        render_buffer_
            .emplace(chunk_pos,
                     Node{QPixmap(kPixmapXInPixels, kPixmapYInPixels), true})
            .first->second;
    RenderChunk(&node.pixmap, map_->GetChunk(chunk_pos));
    return node.pixmap;
  }
  auto& node = render_buffer_.at(chunk_pos);
  node.is_used = true;
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
  for (auto i = render_buffer_.begin(); i != render_buffer_.end();) {
    if (!i->second.is_used) {
      i = render_buffer_.erase(i);
    } else {
      i->second.is_used = false;
      ++i;
    }
  }
}

