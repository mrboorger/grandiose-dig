#ifndef VIEW_TEXTURE_ATLAS_H_
#define VIEW_TEXTURE_ATLAS_H_

#include "model/block.h"
#include "model/constants.h"
#include "view/abstract_atlas.h"

class TextureAtlas : public AbstractAtlas {
 public:
  TextureAtlas();

  static constexpr int kSize = Block::kFrontTypesCount;
  static constexpr int kTextureWidth = kSize * constants::kBlockSz;
  static constexpr int kTextureHeight = constants::kBlockSz;

  void Init() override;

  // Get texture coordinate (TC) of Left/Right Top/Bottom block corner
  // I. e. GetBlockTCLT == Get the texture coordinate of the left top block
  // corner
  static QPointF GetBlockTCLT(Block block);
  static QPointF GetBlockTCLB(Block block);
  static QPointF GetBlockTCRT(Block block);
  static QPointF GetBlockTCRB(Block block);

 private:
  static int32_t GetBlockPixmapXCoordinate(int32_t id);
};

#endif  // VIEW_TEXTURE_ATLAS_H_
