#include "view/texture_atlas.h"

#include <QPainter>

#include "model/constants.h"
#include "view/block_drawer.h"

TextureAtlas::TextureAtlas() : QOpenGLTexture(QOpenGLTexture::Target2D) {}

void TextureAtlas::Init() {
  QPixmap buffer(kTextureWidth, kTextureHeight);
  QPainter painter(&buffer);
  painter.fillRect(0, 0, kTextureWidth, kTextureHeight, Qt::white);
  for (int i = Block::kFirstType; i != Block::kTypesCount; ++i) {
    QPoint position(GetBlockPixmapXCoordinate(i) - constants::kBlockSz, 0);
    for (int j = 0; j < 3; ++j) {
      BlockDrawer::DrawBlock(&painter, position, Block(Block::Type(i)));
      position.rx() += constants::kBlockSz;
    }
  }

  create();
  setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
  setWrapMode(DirectionT, MirroredRepeat);

  setSize(kTextureWidth, kTextureHeight, 1);
  setFormat(QOpenGLTexture::RGBA8_UNorm);
  allocateStorage();
  setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
          GetImage(buffer).constBits());
}

QImage TextureAtlas::GetImage(const QPixmap& pixmap) {
  QImage image = pixmap.toImage();
  if (image.format() != QImage::Format_RGBA8888) {
    return image.convertToFormat(QImage::Format_RGBA8888);
  }
  return image;
}

int32_t TextureAtlas::GetBlockPixmapXCoordinate(int32_t id) {
  return (3 * id + 1) * constants::kBlockSz;
}

QPointF TextureAtlas::GetBlockTCLT(Block block) {
  QPointF position(GetBlockPixmapXCoordinate(block.GetId()), 0.0);
  position.rx() /= kTextureWidth;
  return position;
}

QPointF TextureAtlas::GetBlockTCLB(Block block) {
  QPointF position(GetBlockPixmapXCoordinate(block.GetId()), 1.0);
  position.rx() /= kTextureWidth;
  return position;
}

QPointF TextureAtlas::GetBlockTCRT(Block block) {
  QPointF position(
      GetBlockPixmapXCoordinate(block.GetId()) + constants::kBlockSz - 1, 0.0);
  position.rx() /= kTextureWidth;
  return position;
}

QPointF TextureAtlas::GetBlockTCRB(Block block) {
  QPointF position(
      GetBlockPixmapXCoordinate(block.GetId()) + constants::kBlockSz - 1, 1.0);
  position.rx() /= kTextureWidth;
  return position;
}
