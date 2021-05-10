#include "view/gl_map_drawer.h"

#include <QDebug>
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <cassert>
#include <utility>

#include "model/constants.h"
#include "view/gl_func.h"
#include "view/texture_atlas.h"

GLMapDrawer::GLMapDrawer(std::shared_ptr<AbstractMap> map)
    : buffers_(constants::kDefaultClearTimeMSec),
      index_buffer_(QOpenGLBuffer::Type::IndexBuffer),
      map_(std::move(map)) {}

void GLMapDrawer::Init() {
  GenerateIndexBuffer(&index_buffer_);
  LoadShader(&shader_);
  atlas_.Init();
}

void GLMapDrawer::DrawMapWithCenter(QPainter* painter, const QPointF& pos,
                                    const QRect& screen_coords) {
  Q_UNUSED(painter);
  auto* gl = GLFunctions::GetInstance();

  atlas_.bind();
  shader_.bind();

  QPoint start = RoundToMeshPos(QPoint(pos.x(), pos.y()) -
                                  QPoint(kFieldOfView, kFieldOfView));
  QPoint finish = RoundToMeshPos(QPoint(pos.x(), pos.y()) +
                                   QPoint(kFieldOfView, kFieldOfView));
  map_->CacheRegion(QRect(start, finish));
  {
    QMatrix4x4 proj_matrix;
    assert(proj_matrix.isIdentity());
    float left =
        -((screen_coords.width() + 1) / 2) / (1.0F * constants::kBlockSz);
    float right =
        (screen_coords.width() / 2 + 1) / (1.0F * constants::kBlockSz);
    float up =
        -((screen_coords.height() + 1) / 2) / (1.0F * constants::kBlockSz);
    float down =
        (screen_coords.height() / 2) / (1.0F * constants::kBlockSz);
    proj_matrix.ortho(left, right, up, down, 1.0F, 100.0F);
    proj_matrix.lookAt({0.0F, 0.0F, 0.0F}, {0.0F, 0.0F, 1.0F},
                       {0.0F, -1.0F, 0.0F});
    shader_.setUniformValue("proj_matrix", proj_matrix);
  }

  index_buffer_.bind();

  for (int32_t x = start.x(); x <= finish.x(); x += kMeshWidth) {
    for (int32_t y = start.y(); y <= finish.y(); y += kMeshHeight) {
      QPointF point = (QPointF(x, y) - pos);
      shader_.setUniformValue("buffer_pos", point);
      auto* mesh = GetMesh(QPoint(x, y));

      mesh->bind();

      gl->glEnableVertexAttribArray(0);
      gl->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                                reinterpret_cast<void*>(0 * sizeof(GLfloat)));
      gl->glEnableVertexAttribArray(1);
      gl->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                                reinterpret_cast<void*>(2 * sizeof(GLfloat)));

      gl->glDrawElements(GL_TRIANGLES, kElementsCount, GL_UNSIGNED_INT,
                         nullptr);
      mesh->release();
    }
  }
  index_buffer_.release();
  shader_.release();
  atlas_.release();
}

void GLMapDrawer::UpdateBlock(QPoint position) {
  auto buffer_pos = RoundToMeshPos(position);
  auto buffer = buffers_.Get(buffer_pos);
  if (!buffer) {
    return;
  }
  QOpenGLBuffer& casted = buffer.value();
  casted.bind();
  Block block = map_->GetBlock(position);
  position -= buffer_pos;
  auto data = GetBlockData(block, position);
  casted.write((position.y() * kMeshWidth + position.x()) * sizeof(BlockData),
               &data, sizeof(BlockData));
  casted.release();
}

QPoint GLMapDrawer::RoundToMeshPos(QPoint p) {
  return QPoint(p.x() - utils::ArithmeticalMod(p.x(), kMeshWidth),
                p.y() - utils::ArithmeticalMod(p.y(), kMeshHeight));
}

QRect GLMapDrawer::GetDrawRegion(QPoint center) const {
  return QRect(center.x() - kFieldOfView, center.y() - kFieldOfView,
               center.x() + kFieldOfView, center.y() + kFieldOfView);
}

void GLMapDrawer::GenerateIndexBuffer(QOpenGLBuffer* index_buffer) {
  std::array<GLuint, kElementsCount> data;
  for (int y = 0; y < kMeshHeight; ++y) {
    for (int x = 0; x < kMeshWidth; ++x) {
      int i = y * kMeshWidth + x;
      data[6 * i + 0] = 4 * i + 0;
      data[6 * i + 1] = 4 * i + 1;
      data[6 * i + 2] = 4 * i + 2;
      data[6 * i + 3] = 4 * i + 1;
      data[6 * i + 4] = 4 * i + 2;
      data[6 * i + 5] = 4 * i + 3;
    }
  }
  InitGLBuffer(index_buffer, data.data(), sizeof(data));
}

void GLMapDrawer::LoadShader(QOpenGLShaderProgram* shader) {
  shader->create();
  shader->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                  ":/resources/shaders/vertex.glsl");
  shader->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                  ":/resources/shaders/fragment.glsl");
  assert(shader->link());
}

QOpenGLBuffer* GLMapDrawer::GetMesh(QPoint buffer_pos) {
  auto found = buffers_.Get(buffer_pos);
  if (!found) {
    QOpenGLBuffer& buffer = buffers_.Insert(
        buffer_pos, QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer));
    GenerateMesh(&buffer, buffer_pos);
    return &buffer;
  }
  QOpenGLBuffer& casted = found.value();
  return &casted;
}

void GLMapDrawer::InitGLBuffer(QOpenGLBuffer* buffer, void* data, size_t size) {
  if (buffer->isCreated()) {
    buffer->destroy();
  }
  buffer->create();
  buffer->bind();
  buffer->allocate(data, size);
  buffer->release();
}

void GLMapDrawer::GenerateMesh(QOpenGLBuffer* buffer, QPoint buffer_pos) {
  Q_UNUSED(buffer_pos);

  std::array<BlockData, kMeshSize> data;
  for (int y = 0; y < kMeshHeight; ++y) {
    for (int x = 0; x < kMeshWidth; ++x) {
      data[y * kMeshWidth + x] = GetBlockData(
          map_->GetBlock(QPoint(buffer_pos.x() + x, buffer_pos.y() + y)),
          QPoint(x, y));
    }
  }

  InitGLBuffer(buffer, data.data(), sizeof(data));
}

GLMapDrawer::BlockData GLMapDrawer::GetBlockData(Block block,
                                                 QPoint block_pos) {
  // TODO(degmuk): This is temporary code.
  BlockData data{};
  if (!block.IsVisible()) {
    data.up_left = data.up_right = data.down_left = data.down_right =
        VertexData{-100.0F, -100.0F, 0.0F, 0.0F};
    return data;
  }
  QPointF tex_coords = TextureAtlas::GetBlockTCLT(block);
  data.up_left = VertexData{
      static_cast<GLfloat>(block_pos.x()),
      static_cast<GLfloat>(block_pos.y()),
      static_cast<GLfloat>(tex_coords.x()),
      static_cast<GLfloat>(tex_coords.y()),
  };
  tex_coords = TextureAtlas::GetBlockTCRT(block);
  data.up_right = VertexData{
      static_cast<GLfloat>(block_pos.x() + 1),
      static_cast<GLfloat>(block_pos.y()),
      static_cast<GLfloat>(tex_coords.x()),
      static_cast<GLfloat>(tex_coords.y()),
  };
  tex_coords = TextureAtlas::GetBlockTCLB(block);
  data.down_left = VertexData{
      static_cast<GLfloat>(block_pos.x()),
      static_cast<GLfloat>(block_pos.y() + 1),
      static_cast<GLfloat>(tex_coords.x()),
      static_cast<GLfloat>(tex_coords.y()),
  };
  tex_coords = TextureAtlas::GetBlockTCRB(block);
  data.down_right = VertexData{
      static_cast<GLfloat>(block_pos.x() + 1),
      static_cast<GLfloat>(block_pos.y() + 1),
      static_cast<GLfloat>(tex_coords.x()),
      static_cast<GLfloat>(tex_coords.y()),
  };
  return data;
}
