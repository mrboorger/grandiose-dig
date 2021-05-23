#ifndef VIEW_BLOCK_DRAWER_H_
#define VIEW_BLOCK_DRAWER_H_

#include <QPainter>
#include <QPointF>

#include "model/block.h"

class BlockDrawer {
 public:
  static constexpr int kCracksStageCount = 3;

  static constexpr int kCrackBoundary1 = 90;
  static constexpr int kCrackBoundary2 = 60;
  static constexpr int kCrackBoundary3 = 30;

  static void DrawBlock(QPainter* painter, QPointF point, Block block);
  static void DrawCracks(QPainter* painter, QPointF point, Block block);

  static void ClearBlock(QPainter* painter, QPointF point);
};

#endif  // VIEW_BLOCK_DRAWER_H_
