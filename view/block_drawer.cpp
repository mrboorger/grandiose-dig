#include "view/block_drawer.h"

#include <QString>
#include <array>
#include <cassert>

#include "model/block.h"
#include "model/constants.h"
#include "model/model.h"

namespace BlockDrawer {

namespace {

const std::array<QString, Block::kFrontTypesCount> kFrontNames = {
    "",
    "dirt.png",
    "grass.png",
    "stone.png",
    "snowy_grass.png",
    "sand.png",
    "sandstone.png",
    "coal_ore.png",
    "iron_ore",
    "shimond_ore.png",
    "firemond_ore.png",
    "stone_bricks.png",
    "clay_block.png",
    "brick_block.png",
    "technical.png",
    "red_light.png",
    "yellow_light.png",
    "green_light.png",
    "blue_light.png"};

const std::array<QString, kBackgroundsCount> kBackgroundNames = {
    "forest_background.png", "sky_background.png", "cave_background.png"};

std::array<QImage, Block::kFrontTypesCount> fronts;
std::array<QImage, kBackgroundsCount> backgrounds;

const QImage& GetFront(int32_t id) {
  if (fronts[id].isNull()) {
    fronts[id] = QImage(":/resources/textures/" + kFrontNames[id]);
  }
  return fronts[id];
}

const QImage& GetBackground(int32_t id) {
  if (backgrounds[id].isNull()) {
    backgrounds[id] = QImage(":/resources/textures/" + kBackgroundNames[id]);
  }
  return backgrounds[id];
}

const std::array<QString, BlockDrawer::kCracksStageCount> kCracksNames = {
    "block_cracks_1.png", "block_cracks_2.png", "block_cracks_3.png"};
std::array<QImage, BlockDrawer::kCracksStageCount> cracks_images;

}  // namespace

Backgrounds BackIdToBackgroundId(Block block, QPoint pos) {
  switch (block.GetBackType()) {
    case Block::BackType::kOverworld: {
      if (pos.y() > Model::GetInstance()->GetMap()->SkyLevel()) {
        return Backgrounds::kForest;
      }
      return Backgrounds::kSky;
    }
    case Block::BackType::kCave: {
      return Backgrounds::kCave;
    }
    default: {
      break;
    }
  }
  return Backgrounds::kSky;
}

void DrawCracks(QPainter* painter, QPointF point, Block block) {
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

void DrawBlockFront(QPainter* painter, QPointF point, Block block) {
  if (block.IsVisible()) {
    painter->drawImage(point, GetFront(block.GetFrontId()));
  }
}

void DrawBlockFront(QPainter* painter, QPoint point, Block block) {
  if (block.IsVisible()) {
    painter->drawImage(point, GetFront(block.GetFrontId()));
  }
}

void DrawBackground(QPainter* painter, QPointF point,
                    Backgrounds background_type) {
  const auto& background = GetBackground(static_cast<int>(background_type));
  painter->drawImage(QRectF(point.x(), point.y(), point.x() + kBackgroundWidth,
                            point.y() + kBackgroundHeight),
                     background);
}

void DrawBackground(QPainter* painter, QPoint point,
                    Backgrounds background_type) {
  const auto& background = GetBackground(static_cast<int>(background_type));
  painter->drawImage(QRect(point.x(), point.y(), point.x() + kBackgroundWidth,
                           point.y() + kBackgroundHeight),
                     background);
}

void ClearBlock(QPainter* painter, QPointF point) {
  painter->eraseRect(point.x(), point.y(), constants::kBlockSz,
                     constants::kBlockSz);
}

}  // namespace BlockDrawer
