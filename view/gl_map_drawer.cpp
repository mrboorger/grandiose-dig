#include "view/gl_map_drawer.h"
#include <qopenglfunctions.h>

#include <QOpenGLFunctions>
#include <cassert>

#include "model/constants.h"

GlMapDrawer::GlMapDrawer(std::shared_ptr<AbstractMap> map)
    : buffers_(constants::kDefaultClearTimeMSec),
      index_buffer_(QOpenGLBuffer::Type::IndexBuffer),
      map_(std::move(map)) {
  GenerateIndexBuffer(&index_buffer_);
  LoadShader(&shader_);
  GenerateVAO(&shader_, &vao_);
}

void GlMapDrawer::DrawMapWithCenter(const QPointF& pos,
                                    const QRect& screen_coords) {
  QPoint start = RoundToMeshPos(QPoint(pos.x(), pos.y()) -
                                  QPoint(kFieldOfView, kFieldOfView));
  QPoint finish = RoundToMeshPos(QPoint(pos.x(), pos.y()) +
                                   QPoint(kFieldOfView, kFieldOfView));
  map_->CacheRegion(QRect(start, finish));
  vao_.bind();
  shader_.bind();
  index_buffer_.bind();
  for (int32_t x = start.x(); x <= finish.x(); x += kMeshWidth) {
    for (int32_t y = start.y(); y <= finish.y(); y += kMeshHeight) {
      QPointF point =
          (QPointF(x, y) - pos) * constants::kBlockSz + screen_coords.center();
      shader_.setUniformValue("buffer_pos", point);
      auto* mesh = GetMesh(QPoint(x, y));
      mesh->bind();
      glDrawElements(GL_TRIANGLES, kElementsCount, GL_INT, nullptr);
    }
  }
  index_buffer_.release();
  shader_.release();
  vao_.release();
}

void GlMapDrawer::UpdateBlock(QPoint pos) {
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

QPoint GlMapDrawer::RoundToMeshPos(QPoint p) {
  return QPoint(p.x() - utils::ArithmeticalMod(p.x(), kMeshWidth),
                p.y() - utils::ArithmeticalMod(p.y(), kMeshHeight));
}

void GlMapDrawer::GenerateIndexBuffer(QOpenGLBuffer* index_buffer) {
  std::array<int, 2 * 3 * kMeshWidth * kMeshHeight> buffer;
  for (int x = 0; x < kMeshWidth; ++x) {
    for (int y = 0; y < kMeshHeight; ++y) {
      int i = 4 * (y * kMeshWidth + x);
      buffer[6 * i + 0] = i + 0;
      buffer[6 * i + 1] = i + 1;
      buffer[6 * i + 2] = i + 2;
      buffer[6 * i + 3] = i + 1;
      buffer[6 * i + 4] = i + 2;
      buffer[6 * i + 5] = i + 3;
    }
  }
  index_buffer->create();
  index_buffer->allocate(buffer.data(), sizeof(buffer[0]) * buffer.size());
}

void GlMapDrawer::LoadShader(QOpenGLShaderProgram* shader) {
  shader->create();
  shader->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                  ":/resources/shaders/vertex.glsl");
  shader->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                  ":/resources/shaders/fragment.glsl");
  assert(shader->link());
}

void GlMapDrawer::GenerateVAO(QOpenGLShaderProgram* shader,
                              QOpenGLVertexArrayObject* vao) {
  vao->create();
  vao->bind();

  shader->bind();

  shader->enableAttributeArray(0);
  shader->setAttributeBuffer(0, GL_FLOAT, sizeof(VertexData), 2, 0);
  shader->setAttributeBuffer(1, GL_FLOAT, sizeof(VertexData), 4, 2);

  shader->release();
  vao->release();
}

QOpenGLBuffer* GlMapDrawer::GetMesh(QPoint buffer_pos) {
  auto found = buffers_.Get(buffer_pos);
  if (!found) {
    auto& buffer = buffers_.Insert(
        buffer_pos, QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer));
    GenerateMesh(&buffer, buffer_pos);
    return &buffer;
  }
  QOpenGLBuffer& casted = found.value();
  return &casted;
}

void GlMapDrawer::GenerateMesh(QOpenGLBuffer* buffer, QPoint buffer_pos) {
  if (buffer->isCreated()) {
    buffer->destroy();
  }
  assert(buffer->create());
  buffer->bind();
  std::array<BlockData, kMeshSize> data;

  for (int y = 0; y < kMeshHeight; ++y) {
    for (int x = 0; x < kMeshWidth; ++x) {
      data[y * kMeshWidth + x] = GetBlockData(
          map_->GetBlock(QPoint(buffer_pos.x() + x, buffer_pos.y() + y)),
          QPoint(x, y));
    }
  }
  buffer->allocate(data.data(), kMeshSize);
  buffer->release();
}

GlMapDrawer::BlockData GlMapDrawer::GetBlockData(Block block,
                                                 QPoint block_pos) {
  BlockData data{};
  // TODO(degmuk): This is temporary code.
  if (!block.IsVisible()) {
    data.up_left = data.up_right = data.down_left = data.down_right =
        VertexData{static_cast<GLfloat>(block_pos.x() * constants::kBlockSz),
                   static_cast<GLfloat>(block_pos.y() * constants::kBlockSz),
                   1.0F,
                   1.0F,
                   1.0F,
                   1.0F};
  } else {
    data.up_left = data.up_right = data.down_left = data.down_right =
        VertexData{static_cast<GLfloat>(block_pos.x() * constants::kBlockSz),
                   static_cast<GLfloat>(block_pos.y() * constants::kBlockSz),
                   1.0F,
                   1.0F,
                   0.0F,
                   1.0F};
  }
  data.up_right.pos_x += 1.0F;
  data.down_right.pos_x += 1.0F;
  data.down_left.pos_y += 1.0F;
  data.down_right.pos_y += 1.0F;
  return data;
}
