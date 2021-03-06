#include "block_drawer.h"

#include "model/constants.h"
#include "model/block.h"

namespace {
std::array<QImage, Block::kTypesCount>
    images = {QImage(), QImage(":resources/textures/grass.png")};
}

void BlockDrawer::DrawBlock(QPainter* painter, QPointF point, Block block) {
  if (block.IsVisible()) {
    painter->drawImage(point, images[block.GetId()]);
  }
}
