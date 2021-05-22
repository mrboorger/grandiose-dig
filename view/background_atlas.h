#ifndef VIEW_BACKGROUND_ATLAS_H_
#define VIEW_BACKGROUND_ATLAS_H_

#include "model/block.h"
#include "model/constants.h"
#include "view/abstract_atlas.h"
#include "view/block_drawer.h"

class BackgroundAtlas : public AbstractAtlas {
 public:
  static constexpr int kSize = Block::kBackTypesCount;
  static constexpr int kBackgroundWidth = kSize * BlockDrawer::kBackgroundWidth;
  static constexpr int kBackgroundHeight = BlockDrawer::kBackgroundHeight;

  BackgroundAtlas() : AbstractAtlas(QOpenGLTexture::Target2D) {}

  void Init() override;

  // Get background texture coordinate (TC) of Left/Right Top/Bottom block
  // corner I. e. GetBlockTCLT == Get the texture coordinate of the left top
  // block corner
  static QPointF GetBackgroundTCLT(Block block, QPoint pos);
  static QPointF GetBackgroundTCLB(Block block, QPoint pos);
  static QPointF GetBackgroundTCRT(Block block, QPoint pos);
  static QPointF GetBackgroundTCRB(Block block, QPoint pos);

 private:
  static QPoint GetBackgroundPixmapCoords(int32_t id,
                                          QPoint pos = QPoint(0, 0));
};

#endif  // VIEW_BACKGROUND_ATLAS_H_
