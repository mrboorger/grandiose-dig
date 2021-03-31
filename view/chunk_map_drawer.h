#ifndef VIEW_CHUNK_MAP_DRAWER_H_
#define VIEW_CHUNK_MAP_DRAWER_H_

#include <QPixmap>
#include <QTimer>
#include <memory>
#include <vector>

#include "model/chunk_map.h"
#include "model/constants.h"
#include "view/abstract_map_drawer.h"

class ChunkMapDrawer : public AbstractMapDrawer {
 public:
  explicit ChunkMapDrawer(std::shared_ptr<ChunkMap> map);

  void DrawMapWithCenter(QPainter* painter, const QPointF& pos,
                         const QRect& screen_coords) override;

 private:
  // In chunks
  static constexpr int32_t kFieldOfView = 1;
  static constexpr int32_t kPixmapXInPixels =
      constants::kBlockSz * Chunk::kWidth;
  static constexpr int32_t kPixmapYInPixels =
      constants::kBlockSz * Chunk::kHeight;
  static constexpr int kClearTimeMSec = 1000;

  struct Node {
    QPixmap pixmap;
    QPoint chunk_pos;
    bool is_used;
  };

  const QPixmap& GetChunkPixmap(QPoint chunk_pos);
  static void RenderChunk(QPixmap* buffer, const Chunk& chunk);

  void ClearUnusedNodes();

  std::vector<Node> render_buffer_;
  std::shared_ptr<ChunkMap> map_;
  QTimer clear_timer_;
};

#endif  // VIEW_CHUNK_MAP_DRAWER_H_
