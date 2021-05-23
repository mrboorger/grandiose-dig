#ifndef VIEW_TEXTURE_ATLAS_H_
#define VIEW_TEXTURE_ATLAS_H_

#include <QOpenGLTexture>

#include "model/block.h"
#include "model/constants.h"

class TextureAtlas : public QOpenGLTexture {
 public:
  TextureAtlas();

  static constexpr int kWidth = 512;
  static constexpr int kHeight = 512;
  // Sometimes fragment shader picks color from neighbour texture because of
  // wrong rounding. By adding this shift, it makes mistakes rarelier.
  static constexpr double kRoundShift = 2.5 / constants::kBlockSz;
  static_assert(kWidth * kHeight >= Block::kTypesCount);

  void Init();

  // Get texture coordinate (TC) of Left/Right Top/Bottom block corner
  // I. e. GetBlockTCLT == Get the texture coordinate of the left top block
  // corner
  static QPointF GetBlockTCLT(Block block);
  static QPointF GetBlockTCLB(Block block);
  static QPointF GetBlockTCRT(Block block);
  static QPointF GetBlockTCRB(Block block);

 private:
  static QImage GetImage(const QPixmap& pixmap);
  static QPointF GetBlockPC(int32_t id);
};

#endif  // VIEW_TEXTURE_ATLAS_H_
