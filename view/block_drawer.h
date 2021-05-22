#ifndef VIEW_BLOCK_DRAWER_H_
#define VIEW_BLOCK_DRAWER_H_

#include <QPainter>
#include <QPointF>

#include "model/block.h"
#include "model/chunk.h"
#include "model/constants.h"

class BlockDrawer {
 public:
  static constexpr int kBackgroundWidth = constants::kBlockSz * Chunk::kWidth;
  static constexpr int kBackgroundHeight = constants::kBlockSz * Chunk::kHeight;

  static void DrawBlockFront(QPainter* painter, QPointF point, Block block);
  static void DrawBlockFront(QPainter* painter, QPoint point, Block block);

  static void DrawBackground(QPainter* painter, QPointF point, Block block);
  static void DrawBackground(QPainter* painter, QPoint point, Block block);

  static void ClearBlock(QPainter* painter, QPointF point);
};

#endif  // VIEW_BLOCK_DRAWER_H_
