#include "view/block_drawer.h"

#include <QString>
#include <array>

#include "model/block.h"
#include "model/constants.h"

namespace {

std::array<QString, Block::kTypesCount> names = {"", "dirt.png", "grass.png"};
std::array<QImage, Block::kTypesCount> images;

}  // namespace

void BlockDrawer::DrawBlock(QPainter* painter, QPointF point, Block block) {
  if (block.IsVisible()) {
    if (images[block.GetId()].isNull()) {
      images[block.GetId()] =
          QImage(":/resources/textures/" + names[block.GetId()]);
    }
    painter->drawImage(point, images[block.GetId()]);
  }
}

void BlockDrawer::ClearBlock(QPainter* painter, QPointF point) {
  painter->eraseRect(point.x(), point.y(), constants::kBlockSz,
                     constants::kBlockSz);
}
