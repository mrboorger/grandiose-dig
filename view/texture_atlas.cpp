#include "view/texture_atlas.h"

#include <QPainter>

#include "view/block_drawer.h"

TextureAtlas::TextureAtlas() : QOpenGLTexture(QOpenGLTexture::Target2D) {}

void TextureAtlas::Init() {
  QPixmap buffer(kWidth * constants::kBlockSz, kHeight * constants::kBlockSz);
  QPainter painter(&buffer);
  for (int i = Block::kFirstType; i != Block::kTypesCount; ++i) {
    BlockDrawer::DrawBlock(&painter, GetBlockPC(i), Block(Block::Type(i)));
  }

  create();
  setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);

  setSize(kWidth * constants::kBlockSz, kHeight * constants::kBlockSz, 1);
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

QPointF TextureAtlas::GetBlockPC(int32_t id){
  return QPointF((id % kWidth), (id / kWidth)) * constants::kBlockSz;
}

QPointF TextureAtlas::GetBlockTCLT(Block block) {
  return QPointF(static_cast<double>((block.GetId() + 1) % kWidth) / kWidth,
                 static_cast<double>(block.GetId() / kWidth) / kHeight);
}

QPointF TextureAtlas::GetBlockTCLB(Block block) {
  return QPointF(static_cast<double>((block.GetId() + 1) % kWidth) / kWidth,
                 static_cast<double>(block.GetId() / kWidth + 1) / kHeight);
}

QPointF TextureAtlas::GetBlockTCRT(Block block) {
  return QPointF(
      static_cast<double>(block.GetId() % kWidth + kRoundShift) / kWidth,
      static_cast<double>(block.GetId() / kWidth) / kHeight);
}

QPointF TextureAtlas::GetBlockTCRB(Block block) {
  return QPointF(
      static_cast<double>(block.GetId() % kWidth + kRoundShift) / kWidth,
      static_cast<double>(block.GetId() / kWidth + 1) / kHeight);
}
