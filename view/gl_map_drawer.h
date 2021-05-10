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
#include "view/texture_atlas.h"

class GLMapDrawer : public AbstractMapDrawer {
 public:
  explicit GLMapDrawer(std::shared_ptr<AbstractMap> map);

  void Init() override;

  void DrawMapWithCenter(QPainter* painter, const QPointF& pos,
                         const QRect& screen_coords) override;

  void UpdateBlock(QPoint position) override;

  QRect GetDrawRegion(QPoint center) const override;

 private:
  // In blocks
  static constexpr int32_t kFieldOfView = 64;
  static constexpr int32_t kMeshWidth = 32;
  static constexpr int32_t kMeshHeight = 32;
  static constexpr int32_t kMeshSize = kMeshWidth * kMeshHeight;
  static constexpr int32_t kElementsCount = 2 * 3 * kMeshSize;

  struct VertexData {
    GLfloat pos_x;
    GLfloat pos_y;
    GLfloat tex_u;
    GLfloat tex_v;
  };

  struct BlockData {
    VertexData up_left;
    VertexData up_right;
    VertexData down_left;
    VertexData down_right;
  };

  static QPoint RoundToMeshPos(QPoint p);

  static void GenerateIndexBuffer(QOpenGLBuffer* index_buffer);
  static void LoadShader(QOpenGLShaderProgram* shader);

  QOpenGLBuffer* GetMesh(QPoint buffer_pos);
  void GenerateMesh(QOpenGLBuffer* buffer, QPoint buffer_pos);
  static BlockData GetBlockData(Block block, QPoint block_pos);

  static void InitGLBuffer(QOpenGLBuffer* buffer, void* data, size_t size);

  containers::ClearableCache<QPoint, QOpenGLBuffer,
                             utils::QPointLexicographicalCompare>
      buffers_;
  QOpenGLBuffer index_buffer_;
  QOpenGLShaderProgram shader_;
  TextureAtlas atlas_;
  std::shared_ptr<AbstractMap> map_;
};

#endif  // VIEW_GL_MAP_DRAWER_H_
