#include "view/gl_map_drawer.h"

#include <QDebug>
#include <QOpenGLFunctions>
#include <cassert>

#include "model/constants.h"
#include "view/gl_func.h"

GLMapDrawer::GLMapDrawer(std::shared_ptr<AbstractMap> map)
    : buffers_(constants::kDefaultClearTimeMSec),
      index_buffer_(QOpenGLBuffer::Type::IndexBuffer),
      map_(std::move(map)) {}

void GLMapDrawer::Init() {
  GenerateIndexBuffer(&index_buffer_);
  LoadShader(&shader_);
}

void GLMapDrawer::DrawMapWithCenter(const QPointF& pos,
                                    const QRect& screen_coords) {
  auto* gl = GLFunctions::GetInstance();

  shader_.bind();

  QPoint start = RoundToMeshPos(QPoint(pos.x(), pos.y()) -
                                  QPoint(kFieldOfView, kFieldOfView));
  QPoint finish = RoundToMeshPos(QPoint(pos.x(), pos.y()) +
                                   QPoint(kFieldOfView, kFieldOfView));
  map_->CacheRegion(QRect(start, finish));
//  shader_.setUniformValue(
//      "screen_width",
//      static_cast<GLfloat>(screen_coords.width()) / constants::kBlockSz);
  shader_.setUniformValue(
      "screen_width",
      static_cast<GLfloat>(screen_coords.height()) / constants::kBlockSz);
  shader_.setUniformValue(
      "screen_height",
      static_cast<GLfloat>(screen_coords.height()) / constants::kBlockSz);

  index_buffer_.bind();

  for (int32_t x = start.x(); x <= finish.x(); x += kMeshWidth) {
    for (int32_t y = start.y(); y <= finish.y(); y += kMeshHeight) {
      QPointF point = (QPointF(x, y) -
                       pos) /*+ screen_coords.center() / constants::kBlockSz*/;
      shader_.setUniformValue("buffer_pos", point);
      auto* mesh = GetMesh(QPoint(x, y));

      mesh->bind();

      gl->glEnableVertexAttribArray(0);
      gl->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                                (void*)(0 * sizeof(GLfloat)));
      gl->glEnableVertexAttribArray(1);
      gl->glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                                (void*)(2 * sizeof(GLfloat)));

      gl->glDrawElements(GL_TRIANGLES, kElementsCount, GL_UNSIGNED_INT,
                         nullptr);
      mesh->release();
    }
  }
  index_buffer_.release();
  shader_.release();
}

void GLMapDrawer::UpdateBlock(QPoint pos) {
  auto buffer_pos = RoundToMeshPos(pos);
  auto buffer = buffers_.Get(buffer_pos);
  if (!buffer) {
    return;
  }
  QOpenGLBuffer& casted = buffer.value();
  casted.bind();
  Block block = map_->GetBlock(pos);
  auto data = GetBlockData(block, pos - RoundToMeshPos(pos));
  casted.write((pos.y() * kMeshWidth + pos.x()) * sizeof(BlockData), &data,
               sizeof(BlockData));
  casted.release();
}

QPoint GLMapDrawer::RoundToMeshPos(QPoint p) {
  return QPoint(p.x() - utils::ArithmeticalMod(p.x(), kMeshWidth),
                p.y() - utils::ArithmeticalMod(p.y(), kMeshHeight));
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
        VertexData{static_cast<GLfloat>(block_pos.x()),
                   static_cast<GLfloat>(block_pos.y()),
                   0.0F,
                   0.0F,
                   0.0F,
                   1.0F};
  } else {
    data.up_left = data.up_right = data.down_left = data.down_right =
        VertexData{static_cast<GLfloat>(block_pos.x()),
                   static_cast<GLfloat>(block_pos.y()),
                   1.0F,
                   1.0F,
                   0.0F,
                   1.0F};
  }
  data.up_right.pos_x += 1.0F;
  data.down_right.pos_x += 1.0F;
  data.up_left.pos_y += 1.0F;
  data.up_right.pos_y += 1.0F;
  return data;
}
