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
    "", "dirt.png", "grass.png"};

const std::array<QString, kBackgroundsCount> kBackgroundNames = {
    "forest_background.png", "sky_background.png", "cave_background.png"};

std::array<QImage, Block::kFrontTypesCount> fronts;
std::array<QImage, kBackgroundsCount> backgrounds;

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
      assert(false);
    }
  }
}

void DrawBlockFront(QPainter* painter, QPointF point,
                                 Block block) {
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
  assert(!background.isNull());
  painter->drawImage(QRectF(point.x(), point.y(), point.x() + kBackgroundWidth,
                            point.y() + kBackgroundHeight),
                     background);
}

void DrawBackground(QPainter* painter, QPoint point,
                    Backgrounds background_type) {
  const auto& background = GetBackground(static_cast<int>(background_type));
  assert(!background.isNull());
  painter->drawImage(QRect(point.x(), point.y(), point.x() + kBackgroundWidth,
                           point.y() + kBackgroundHeight),
                     background);
}

void ClearBlock(QPainter* painter, QPointF point) {
  painter->eraseRect(point.x(), point.y(), constants::kBlockSz,
                     constants::kBlockSz);
}

}  // namespace BlockDrawer
