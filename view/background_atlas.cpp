#include "view/background_atlas.h"

#include "model/constants.h"
#include "utils.h"
#include "view/block_drawer.h"

void BackgroundAtlas::Init() {
  QPixmap buffer(kPixmapWidth, kPixmapHeight);
  QPainter painter(&buffer);
  painter.fillRect(0, 0, kPixmapWidth, kPixmapHeight, Qt::white);
  for (int i = BlockDrawer::kBackgroundsFirst;
       i != BlockDrawer::kBackgroundsCount; ++i) {
    BlockDrawer::DrawBackground(&painter, GetBackgroundPixmapCoords(i),
                                BlockDrawer::Backgrounds(i));
  }

  create();
  setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
  setWrapMode(DirectionS, Repeat);

  setSize(kPixmapWidth, kPixmapHeight, 1);
  setFormat(QOpenGLTexture::RGBA8_UNorm);
  allocateStorage();
  setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
          GetImage(buffer).constBits());
}

QPoint BackgroundAtlas::GetBackgroundPixmapCoords(int32_t id, QPoint pos) {
  pos.rx() = pos.x() * constants::kBlockSz;
  pos.ry() = id * BlockDrawer::kBackgroundHeight +
             utils::ArithmeticalMod(pos.y() * constants::kBlockSz,
                                    BlockDrawer::kBackgroundHeight);
  return pos;
}

QPointF BackgroundAtlas::GetBackgroundTCLT(Block block, QPoint pos) {
  int back_id = static_cast<int>(BlockDrawer::BackIdToBackgroundId(block, pos));
  QPointF result(GetBackgroundPixmapCoords(back_id, pos));
  result.rx() /= kPixmapWidth;
  result.ry() /= kPixmapHeight;
  return result;
}

QPointF BackgroundAtlas::GetBackgroundTCLB(Block block, QPoint pos) {
  int back_id = static_cast<int>(BlockDrawer::BackIdToBackgroundId(block, pos));
  QPointF result(GetBackgroundPixmapCoords(back_id, pos));
  result.rx() /= kPixmapWidth;
  result.ry() = (result.ry() + constants::kBlockSz - 1) / kPixmapHeight;
  return result;
}

QPointF BackgroundAtlas::GetBackgroundTCRT(Block block, QPoint pos) {
  int back_id = static_cast<int>(BlockDrawer::BackIdToBackgroundId(block, pos));
  QPointF result(GetBackgroundPixmapCoords(back_id, pos));
  result.rx() = (result.rx() + constants::kBlockSz - 1) / kPixmapWidth;
  result.ry() /= kPixmapHeight;
  return result;
}

QPointF BackgroundAtlas::GetBackgroundTCRB(Block block, QPoint pos) {
  int back_id = static_cast<int>(BlockDrawer::BackIdToBackgroundId(block, pos));
  QPointF result(GetBackgroundPixmapCoords(back_id, pos));
  result.rx() = (result.rx() + constants::kBlockSz - 1) / kPixmapWidth;
  result.ry() = (result.ry() + constants::kBlockSz - 1) / kPixmapHeight;
  return result;
}
