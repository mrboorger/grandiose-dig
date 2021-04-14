#ifndef VIEW_BLOCK_DRAWER_H_
#define VIEW_BLOCK_DRAWER_H_

#include <QPainter>
#include <QPointF>

#include "model/block.h"

class BlockDrawer {
 public:
  static void DrawBlock(QPainter* painter, QPointF point, Block block);

  static void ClearBlock(QPainter* painter, QPointF point);
};

#endif  // VIEW_BLOCK_DRAWER_H_
