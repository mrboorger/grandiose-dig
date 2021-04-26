#ifndef VIEW_GL_MAP_DRAWER_H_
#define VIEW_GL_MAP_DRAWER_H_

#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <memory>

#include "model/abstract_map.h"
#include "model/clearable_cache.h"
#include "model/constants.h"
#include "utils.h"

class GlMapDrawer : protected QOpenGLFunctions {
 public:
  explicit GlMapDrawer(std::shared_ptr<AbstractMap> map);

  void DrawMapWithCenter(const QPointF& pos, const QRect& screen_coords);

  void UpdateBlock(QPoint pos);

 private:
  // In blocks
  static constexpr int32_t kFieldOfView = 64;
  static constexpr int32_t kMeshWidth = 32;
  static constexpr int32_t kMeshHeight = 32;

  struct VertexData {
    GLfloat pos_x;
    GLfloat pos_y;
    GLfloat color_r;
    GLfloat color_g;
    GLfloat color_b;
    GLfloat color_a;
  };

  struct BlockData {
    VertexData up_left;
    VertexData up_right;
    VertexData down_left;
    VertexData down_right;
  };

  static constexpr int32_t kMeshSize =
      kMeshWidth * kMeshHeight * sizeof(BlockData);
  static constexpr int32_t kElementsCount =
      2 * kMeshWidth * kMeshHeight;

  static QPoint RoundToMeshPos(QPoint p);

  static void GenerateIndexBuffer(QOpenGLBuffer* index_buffer);
  static void LoadShader(QOpenGLShaderProgram* shader);
  static void GenerateVAO(QOpenGLShaderProgram* shader,
                          QOpenGLVertexArrayObject* vao);

  QOpenGLBuffer* GetMesh(QPoint buffer_pos);
  void GenerateMesh(QOpenGLBuffer* buffer, QPoint buffer_pos);
  static BlockData GetBlockData(Block block, QPoint block_pos);

  containers::ClearableCache<QPoint, QOpenGLBuffer,
                             utils::QPointLexicographicalCompare>
      buffers_;
  QOpenGLBuffer index_buffer_;
  QOpenGLVertexArrayObject vao_;
  QOpenGLShaderProgram shader_;
  std::shared_ptr<AbstractMap> map_;
};

#endif  // VIEW_GL_MAP_DRAWER_H_
