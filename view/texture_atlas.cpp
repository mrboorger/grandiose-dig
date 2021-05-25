#include "view/texture_atlas.h"

#include <QPainter>

#include "model/constants.h"
#include "view/block_drawer.h"

TextureAtlas::TextureAtlas() : AbstractAtlas(QOpenGLTexture::Target2D) {}

void TextureAtlas::Init() {
  QPixmap buffer(kTextureWidth, kTextureHeight);
  QPainter painter(&buffer);
  painter.fillRect(0, 0, kTextureWidth, kTextureHeight, Qt::white);
  for (int i = Block::kFirstFrontType; i != Block::kFrontTypesCount; ++i) {
    QPoint position(GetBlockPixmapXCoordinate(i), 0);
    BlockDrawer::DrawBlockFront(&painter, position, Block(Block::FrontType(i)));
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

int32_t TextureAtlas::GetBlockPixmapXCoordinate(int32_t id) {
  return id * constants::kBlockSz;
}

QPointF TextureAtlas::GetBlockTCLT(Block block) {
  QPointF position(GetBlockPixmapXCoordinate(block.GetFrontId()), 0.0);
  position.rx() /= kTextureWidth;
  return position;
}

QPointF TextureAtlas::GetBlockTCLB(Block block) {
  QPointF position(GetBlockPixmapXCoordinate(block.GetFrontId()), 1.0);
  position.rx() /= kTextureWidth;
  return position;
}

QPointF TextureAtlas::GetBlockTCRT(Block block) {
  QPointF position(
      GetBlockPixmapXCoordinate(block.GetFrontId()) + constants::kBlockSz - 1,
      0.0);
  position.rx() /= kTextureWidth;
  return position;
}

QPointF TextureAtlas::GetBlockTCRB(Block block) {
  QPointF position(
      GetBlockPixmapXCoordinate(block.GetFrontId()) + constants::kBlockSz - 1,
      1.0);
  position.rx() /= kTextureWidth;
  return position;
}
