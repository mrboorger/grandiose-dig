#ifndef VIEW_BLOCK_DRAWER_H_
#define VIEW_BLOCK_DRAWER_H_

#include <QPointF>
#include <QPainter>
#include <array>

#include "model/block.h"

class BlockDrawer {
 public:
  static void DrawBlock(QPainter* painter, QPointF point, Block block);
 private:
};

#endif  // VIEW_BLOCK_DRAWER_H_
