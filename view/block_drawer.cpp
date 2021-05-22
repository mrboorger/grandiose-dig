#include "view/block_drawer.h"

#include <QString>
#include <array>

#include "model/block.h"
#include "model/constants.h"

namespace {

const std::array<QString, Block::kFrontTypesCount> kFrontNames = {
    "", "dirt.png", "grass.png"};
const std::array<QString, Block::kFrontTypesCount> kBackgroundNames = {
    "sky_background.png", "cave_background.png"};

std::array<QImage, Block::kFrontTypesCount> fronts;
std::array<QImage, Block::kBackTypesCount> backgrounds;

const QImage& GetFront(int32_t id) {
  if (fronts[id].isNull()) {
    fronts[id] =
        QImage(":/resources/textures/" + kFrontNames[id]);
  }
  return fronts[id];
}

const QImage& GetBackground(int32_t id) {
  if (backgrounds[id].isNull()) {
    backgrounds[id] = QImage(":/resources/textures/" + kBackgroundNames[id]);
  }
  return backgrounds[id];
}

}  // namespace

void BlockDrawer::DrawBlockFront(QPainter* painter, QPointF point,
                                 Block block) {
  if (block.IsVisible()) {
    painter->drawImage(point, GetFront(block.GetFrontId()));
  }
}

void BlockDrawer::DrawBlockFront(QPainter* painter, QPoint point, Block block) {
  if (block.IsVisible()) {
    painter->drawImage(point, GetFront(block.GetFrontId()));
  }
}

void BlockDrawer::DrawBackground(QPainter* painter, QPointF point,
                                 Block block) {
  const auto& background = GetBackground(block.GetBackId());
  painter->drawImage(QRectF(point.x(), point.y(), point.x() + kBackgroundWidth,
                            point.y() + kBackgroundHeight),
                     background);
}

void BlockDrawer::DrawBackground(QPainter* painter, QPoint point, Block block) {
  const auto& background = GetBackground(block.GetBackId());
  painter->drawImage(QRect(point.x(), point.y(), point.x() + kBackgroundWidth,
                           point.y() + kBackgroundHeight),
                     background);
}

void BlockDrawer::ClearBlock(QPainter* painter, QPointF point) {
  painter->eraseRect(point.x(), point.y(), constants::kBlockSz,
                     constants::kBlockSz);
}
