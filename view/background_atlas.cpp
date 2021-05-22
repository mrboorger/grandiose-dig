#include "view/background_atlas.h"

#include "model/constants.h"
#include "utils.h"

#include <QFile>

void BackgroundAtlas::Init() {
  QPixmap buffer(kBackgroundWidth, kBackgroundHeight);
  QPainter painter(&buffer);
  painter.fillRect(0, 0, kBackgroundWidth, kBackgroundHeight, Qt::white);
  for (int i = Block::kFirstBackType; i != Block::kBackTypesCount; ++i) {
    BlockDrawer::DrawBackground(
        &painter, GetBackgroundPixmapCoords(i),
        Block(Block::FrontType::kAir, Block::BackType(i)));
  }

  QFile file("yourFile.png");
  file.open(QIODevice::WriteOnly);
  buffer.save(&file, "PNG");

  create();
  setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
  setWrapMode(DirectionT, MirroredRepeat);

  setSize(kBackgroundWidth, kBackgroundHeight, 1);
  setFormat(QOpenGLTexture::RGBA8_UNorm);
  allocateStorage();
  setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
          GetImage(buffer).constBits());
}

QPoint BackgroundAtlas::GetBackgroundPixmapCoords(int32_t id, QPoint pos) {
  pos.rx() = id * BlockDrawer::kBackgroundWidth +
             utils::ArithmeticalMod(pos.x() * constants::kBlockSz,
                                    BlockDrawer::kBackgroundWidth);
  pos.ry() = utils::ArithmeticalMod(pos.y() * constants::kBlockSz,
                                    BlockDrawer::kBackgroundHeight);
  return pos;
}

QPointF BackgroundAtlas::GetBackgroundTCLT(Block block, QPoint pos) {
  QPointF result(GetBackgroundPixmapCoords(block.GetBackId(), pos));
  result.rx() /= kBackgroundWidth;
  result.ry() /= kBackgroundHeight;
  return result;
}

QPointF BackgroundAtlas::GetBackgroundTCLB(Block block, QPoint pos) {
  QPointF result(GetBackgroundPixmapCoords(block.GetBackId(), pos));
  result.rx() /= kBackgroundWidth;
  result.ry() = (result.ry() + constants::kBlockSz - 1) / kBackgroundHeight;
  return result;
}

QPointF BackgroundAtlas::GetBackgroundTCRT(Block block, QPoint pos) {
  QPointF result(GetBackgroundPixmapCoords(block.GetBackId(), pos));
  result.rx() = (result.rx() + constants::kBlockSz - 1) / kBackgroundWidth;
  result.ry() /= kBackgroundHeight;
  return result;
}

QPointF BackgroundAtlas::GetBackgroundTCRB(Block block, QPoint pos) {
  QPointF result(GetBackgroundPixmapCoords(block.GetBackId(), pos));
  result.rx() = (result.rx() + constants::kBlockSz - 1) / kBackgroundWidth;
  result.ry() = (result.ry() + constants::kBlockSz - 1) / kBackgroundHeight;
  return result;
}
