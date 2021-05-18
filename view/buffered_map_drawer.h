#ifndef VIEW_BUFFERED_MAP_DRAWER_H_
#define VIEW_BUFFERED_MAP_DRAWER_H_

#include <QPixmap>
#include <QTimer>
#include <map>
#include <memory>
#include <vector>

#include "model/abstract_map.h"
#include "model/clearable_cache.h"
#include "model/constants.h"
#include "utils.h"
#include "view/abstract_map_drawer.h"

class BufferedMapDrawer : public AbstractMapDrawer {
 public:
  explicit BufferedMapDrawer(std::shared_ptr<AbstractMap> map);

  void DrawMapWithCenter(QPainter* painter, const QPointF& pos,
                         const QRect& screen_coords) override;

  void UpdateBlock(QPoint pos) override;

  QRect GetDrawRegion(QPoint center) const override;

 private:
  // In blocks
  static constexpr int32_t kFieldOfView = 64;
  static constexpr int32_t kBufferWidth = 32;
  static constexpr int32_t kBufferHeight = 32;
  static constexpr int32_t kPixmapWidthInPixels =
      constants::kBlockSz * kBufferWidth;
  static constexpr int32_t kPixmapHeightInPixels =
      constants::kBlockSz * kBufferHeight;

  static QPoint RoundToBufferPos(QPoint p);

  const QPixmap& GetBufferPixmap(QPoint buffer_pos);
  static void RenderBlock(QPainter* painter, QPointF block_drawer_pos,
                          Block block, bool need_reset = false);
  void RenderBuffer(QPixmap* buffer, QPoint buffer_pos);

  containers::ClearableCache<QPoint, QPixmap,
                             utils::QPointLexicographicalCompare>
      buffers_;
  std::shared_ptr<AbstractMap> map_;
};

#endif  // VIEW_BUFFERED_MAP_DRAWER_H_
