#ifndef VIEW_GL_MAP_DRAWER_H_
#define VIEW_GL_MAP_DRAWER_H_

#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <memory>

#include "model/abstract_map.h"
#include "model/clearable_cache.h"
#include "model/constants.h"
#include "utils.h"
#include "view/abstract_map_drawer.h"
#include "view/background_atlas.h"
#include "view/gl_func.h"
#include "view/light_map.h"
#include "view/texture_atlas.h"

class GLMapDrawer : public AbstractMapDrawer {
 public:
  explicit GLMapDrawer(std::shared_ptr<AbstractMap> map,
                       std::shared_ptr<LightMap> light_map);

 private:
  struct VertexData {
    GLfloat pos_x;
    GLfloat pos_y;
    GLfloat tex_z;
    GLfloat back_z;
    GLfloat tex_u;
    GLfloat tex_v;
    GLfloat back_u;
    GLfloat back_v;
    GLfloat light_r;
    GLfloat light_g;
    GLfloat light_b;
    GLfloat light_sun;
    GLfloat pos_shift;
  };

  struct BlockData {
    VertexData left_top;
    VertexData right_top;
    VertexData right_bottom;
    VertexData left_bottom;
    VertexData center;
  };

  void InitImpl() override;

  void DrawMapWithCenterImpl(QPainter* painter, const QPointF& pos,
                             const QRect& screen_coords) override;

  void UpdateBlockImpl(QPoint position) override;

  QRect GetDrawRegionImpl(QPoint center) const override;


  // In blocks
  static constexpr int32_t kFieldOfView = 64;
  static constexpr int32_t kMeshWidth = 32;
  static constexpr int32_t kMeshHeight = 32;
  static constexpr int32_t kMeshSize = kMeshWidth * kMeshHeight;
  static constexpr int32_t kElementsPerBlock = 4 * 3;
  static constexpr int32_t kElementsCount = kElementsPerBlock * kMeshSize;
  static constexpr int kAttribsCount = 6;
  static constexpr std::array<int, kAttribsCount> kAttribSizes{2, 1, 2,
                                                               3, 1, 1};
  static constexpr std::array<int, kAttribsCount> kBlockStrides{0, 0, 1,
                                                                2, 0, 0};
  static constexpr std::array<int, kAttribsCount> kBackgroundStrides{0, 1, 2,
                                                                     0, 0, 0};
  static constexpr double kNotVisibleZ = 0.0;
  static constexpr double kBackgroundZ = 1.1;
  static constexpr double kBlocksZ = 1.0;
  static constexpr std::array<const char*, kAttribsCount> kAttribNames{
      "in_pos", "in_z_coord", "in_tex_coords", "in_light", "in_sun"};
  static constexpr VertexData kNoDrawVertex = VertexData{};
  static constexpr BlockData kNoDrawBlockData = BlockData{
      kNoDrawVertex, kNoDrawVertex, kNoDrawVertex, kNoDrawVertex, kNoDrawVertex,
  };
  static constexpr int32_t kVerticesPerBlock =
      sizeof(BlockData) / sizeof(VertexData);

  static VertexData GenData(QPoint pos, double z, QPointF tex_coords,
                            QPointF back_coords, Light light,
                            double pos_shift);

  static QPoint RoundToMeshPos(QPoint p);
  static VertexData Average(const VertexData& a, const VertexData& b,
                            const VertexData& c, const VertexData& d);

  static void GenerateIndexBuffer(QOpenGLBuffer* index_buffer);
  static void LoadShader(QOpenGLShaderProgram* shader);

  QOpenGLBuffer* GetMesh(QPoint buffer_pos);
  void GenerateMesh(QOpenGLBuffer* buffer, QPoint buffer_pos);

  BlockData GetBlockData(QPoint world_pos, QPoint mesh_pos);

  static void InitGLBuffer(QOpenGLBuffer* buffer, void* data, size_t size);

  containers::ClearableCache<QPoint, QOpenGLBuffer,
                             utils::QPointLexicographicalCompare>
      buffers_;
  QOpenGLBuffer index_buffer_;
  QOpenGLShaderProgram shader_;
  TextureAtlas block_textures_;
  BackgroundAtlas backgrounds_;
  std::shared_ptr<AbstractMap> map_;
  std::shared_ptr<LightMap> light_map_;
};

#endif  // VIEW_GL_MAP_DRAWER_H_
