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
#include "view/gl_func.h"
#include "view/light_map.h"
#include "view/texture_atlas.h"

class GLMapDrawer : public AbstractMapDrawer {
 public:
  explicit GLMapDrawer(std::shared_ptr<AbstractMap> map,
                       std::shared_ptr<LightMap> light_map);

  void Init() override;

  void DrawMapWithCenter(QPainter* painter, const QPointF& pos,
                         const QRect& screen_coords) override;

  void UpdateBlock(QPoint position) override;

  QRect GetDrawRegion(QPoint center) const override;

 private:
  struct VertexData {
    GLfloat pos_x;
    GLfloat pos_y;
    GLfloat tex_u;
    GLfloat tex_v;
    GLfloat light_r;
    GLfloat light_g;
    GLfloat light_b;
    GLfloat light_sun;
  };

  struct BlockData {
    VertexData up_left;
    VertexData up_right;
    VertexData down_right;
    VertexData down_left;
    VertexData center;
  };

  // In blocks
  static constexpr int32_t kFieldOfView = 64;
  static constexpr int32_t kMeshWidth = 32;
  static constexpr int32_t kMeshHeight = 32;
  static constexpr int32_t kMeshSize = kMeshWidth * kMeshHeight;
  static constexpr int32_t kElementsPerBlock = 4 * 3;
  static constexpr int32_t kElementsCount = kElementsPerBlock * kMeshSize;
  static constexpr int kAttribsCount = 4;
  static constexpr std::array<int, kAttribsCount> kAttribSizes{2, 2, 3, 1};
  static constexpr VertexData kNoDrawVertex = VertexData{};
  static constexpr BlockData kNoDrawBlockData = BlockData{
      kNoDrawVertex, kNoDrawVertex, kNoDrawVertex, kNoDrawVertex, kNoDrawVertex,
  };
  static constexpr int32_t kVerticesPerBlock =
      sizeof(BlockData) / sizeof(VertexData);

  static VertexData GenData(QPoint pos, QPointF tex_coords, Light light);

  static QPoint RoundToMeshPos(QPoint p);
  static GLfloat Average(GLfloat a, GLfloat b, GLfloat c, GLfloat d);

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
  TextureAtlas atlas_;
  std::shared_ptr<AbstractMap> map_;
  std::shared_ptr<LightMap> light_map_;
};

#endif  // VIEW_GL_MAP_DRAWER_H_
