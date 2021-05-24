#include "view/block_drawer.h"

#include <QString>
#include <array>

#include "model/block.h"
#include "model/constants.h"

namespace {

const std::array<QString, Block::kTypesCount> kNames = {"", "dirt.png",
                                                        "grass.png"};
std::array<QImage, Block::kTypesCount> images;

const std::array<QString, BlockDrawer::kCracksStageCount> kCracksNames = {
    "block_cracks_1.png", "block_cracks_2.png", "block_cracks_3.png"};
std::array<QImage, BlockDrawer::kCracksStageCount> cracks_images;

}  // namespace

void BlockDrawer::DrawBlock(QPainter* painter, QPointF point, Block block) {
  if (block.IsVisible()) {
    if (images[block.GetId()].isNull()) {
      images[block.GetId()] =
          QImage(":/resources/textures/" + kNames[block.GetId()]);
    }
    painter->drawImage(point, images[block.GetId()]);
    DrawCracks(painter, point, block);
  }
}

void BlockDrawer::DrawCracks(QPainter* painter, QPointF point, Block block) {
  int durability_percentage = block.GetRemainingDurabilityPercentage();
  int crack_num = 0;
  if (durability_percentage < kCrackBoundary3) {
    crack_num = 2;
  } else if (durability_percentage < kCrackBoundary2) {
    crack_num = 1;
  } else if (durability_percentage < kCrackBoundary1) {
    crack_num = 0;
  } else {
    return;
  }
  if (cracks_images[crack_num].isNull()) {
    cracks_images[crack_num] =
        QImage(":/resources/textures/" + kCracksNames[crack_num]);
  }
  painter->drawImage(point, cracks_images[crack_num]);
}

void BlockDrawer::ClearBlock(QPainter* painter, QPointF point) {
  painter->eraseRect(point.x(), point.y(), constants::kBlockSz,
                     constants::kBlockSz);
}
