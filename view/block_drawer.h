#ifndef VIEW_BLOCK_DRAWER_H_
#define VIEW_BLOCK_DRAWER_H_

#include <QPainter>
#include <QPointF>

#include "model/block.h"
#include "model/chunk.h"
#include "model/constants.h"

namespace BlockDrawer {

enum class Backgrounds {
  kFirst,
  kForest = kFirst,
  kSky,
  kSpace,
  kCave,
  kBackgroundsCount
};

constexpr int kBackgroundsFirst =
    static_cast<int>(Backgrounds::kFirst);

constexpr int kBackgroundsCount =
    static_cast<int>(Backgrounds::kBackgroundsCount);

constexpr int kBackgroundWidth = constants::kBlockSz * Chunk::kWidth;
constexpr int kBackgroundHeight = constants::kBlockSz * Chunk::kHeight;

void DrawBlockFront(QPainter* painter, QPointF point, Block block);
void DrawBlockFront(QPainter* painter, QPoint point, Block block);

Backgrounds BackIdToBackgroundId(Block block, QPoint pos);
void DrawBackground(QPainter* painter, QPointF point,
                    Backgrounds background_type);
void DrawBackground(QPainter* painter, QPoint point,
                    Backgrounds background_type);

void ClearBlock(QPainter* painter, QPointF point);

}  // namespace BlockDrawer

#endif  // VIEW_BLOCK_DRAWER_H_
