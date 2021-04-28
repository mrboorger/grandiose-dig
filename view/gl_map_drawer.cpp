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
  shader_.setUniformValue("screen_width", screen_coords.width());
  shader_.setUniformValue("screen_height", screen_coords.height());
  for (int32_t x = start.x(); x <= finish.x(); x += kMeshWidth) {
    for (int32_t y = start.y(); y <= finish.y(); y += kMeshHeight) {
      QPointF point =
          (QPointF(x, y) - pos) * constants::kBlockSz + screen_coords.center();
      shader_.setUniformValue("buffer_pos", point);
      auto* mesh = GetMesh(QPoint(x, y));
      mesh->bind();
      gl->glBindBuffer(GL_ARRAY_BUFFER, mesh->bufferId());

      gl->glEnableVertexAttribArray(0);
      gl->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                                (void*)(0 * sizeof(GLfloat)));
      gl->glEnableVertexAttribArray(1);
      gl->glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                                (void*)(2 * sizeof(GLfloat)));

      gl->glDrawArrays(GL_TRIANGLES, 0, 6);
//      gl->glDrawElements(GL_TRIANGLES, kElementsCount, GL_INT, nullptr);
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
  std::array<GLuint, kElementsCount> buffer;
  for (int y = 0; y < kMeshHeight; ++y) {
    for (int x = 0; x < kMeshWidth; ++x) {
      int i = 6 * (y * kMeshWidth + x);
      buffer[i + 0] = i + 0;
      buffer[i + 1] = i + 1;
      buffer[i + 2] = i + 2;
      buffer[i + 3] = i + 1;
      buffer[i + 4] = i + 2;
      buffer[i + 5] = i + 3;
    }
  }
  index_buffer->create();
  index_buffer->allocate(buffer.data(), sizeof(buffer[0]) * buffer.size());
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

void GLMapDrawer::GenerateMesh(QOpenGLBuffer* buffer, QPoint buffer_pos) {
  Q_UNUSED(buffer_pos);
  if (buffer->isCreated()) {
    buffer->destroy();
  }
  std::array<float, (2 + 4) * 6> data{
      -0.5F, -0.5F, 1.0F, 0.0F, 0.0F, 1.0F,
      -0.5F,  0.5F, 0.0F, 1.0F, 0.0F, 1.0F,
       0.5F,  0.5F, 0.0F, 0.0F, 1.0F, 1.0F,
      -0.5F, -0.5F, 1.0F, 0.0F, 0.0F, 1.0F,
       0.5F, -0.5F, 0.0F, 1.0F, 0.0F, 1.0F,
       0.5F,  0.5F, 0.0F, 0.0F, 1.0F, 1.0F,
  };
  buffer->create();
  buffer->bind();
  buffer->allocate(data.data(), sizeof(data));
  buffer->release();
  //  std::array<BlockData, kMeshSize> data;
  //
  //  for (int y = 0; y < kMeshHeight; ++y) {
  //    for (int x = 0; x < kMeshWidth; ++x) {
  //      data[y * kMeshWidth + x] = GetBlockData(
  //          map_->GetBlock(QPoint(buffer_pos.x() + x, buffer_pos.y() + y)),
  //          QPoint(x, y));
  //    }
  //  }
  //  buffer->allocate(data.data(), sizeof(data));
  //  buffer->release();
}

GLMapDrawer::BlockData GLMapDrawer::GetBlockData(Block block,
                                                 QPoint block_pos) {
  Q_UNUSED(block);
  Q_UNUSED(block_pos);
  return BlockData{};
  // TODO(degmuk): This is temporary code.
  //  if (!block.IsVisible()) {
  //    data.up_left = data.up_right = data.down_left = data.down_right =
  //        VertexData{static_cast<GLfloat>(block_pos.x() *
  //        constants::kBlockSz),
  //                   static_cast<GLfloat>(block_pos.y() *
  //                   constants::kBlockSz), 0.0F, 0.0F, 0.0F, 1.0F};
  //  } else {
  //    data.up_left = data.up_right = data.down_left = data.down_right =
  //        VertexData{static_cast<GLfloat>(block_pos.x() *
  //        constants::kBlockSz),
  //                   static_cast<GLfloat>(block_pos.y() *
  //                   constants::kBlockSz), 1.0F, 1.0F, 0.0F, 1.0F};
  //  }
  //  data.up_right.pos_x += constants::kBlockSz;
  //  data.down_right.pos_x += constants::kBlockSz;
  //  data.down_left.pos_y += constants::kBlockSz;
  //  data.down_right.pos_y += constants::kBlockSz;
  //  return data;
}
