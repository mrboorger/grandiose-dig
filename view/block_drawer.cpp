#include "view/block_drawer.h"

#include <QString>
#include <array>

#include "model/block.h"
#include "model/constants.h"

namespace {

const std::array<QString, Block::kTypesCount> kNames = {"", "dirt.png",
                                                        "grass.png"};
std::array<QImage, Block::kTypesCount> images;

const QImage& GetTexture(int32_t id) {
  if (images[id].isNull()) {
    images[id] =
        QImage(":/resources/textures/" + kNames[id]);
  }
  return images[id];
}

}  // namespace

void BlockDrawer::DrawBlock(QPainter* painter, QPointF point, Block block) {
  if (block.IsVisible()) {
    painter->drawImage(point, GetTexture(block.GetId()));
  }
}

void BlockDrawer::DrawBlock(QPainter* painter, QPoint point, Block block) {
  if (block.IsVisible()) {
    painter->drawImage(point, GetTexture(block.GetId()));
  }
}

void BlockDrawer::ClearBlock(QPainter* painter, QPointF point) {
  painter->eraseRect(point.x(), point.y(), constants::kBlockSz,
                     constants::kBlockSz);
}
