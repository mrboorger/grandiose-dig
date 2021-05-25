#include "view/gl_map_drawer.h"

#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <algorithm>
#include <cassert>
#include <utility>

#include "model/constants.h"
#include "view/background_atlas.h"
#include "view/block_drawer.h"
#include "view/gl_func.h"
#include "view/texture_atlas.h"

GLMapDrawer::GLMapDrawer(const QString& save_file,
                         std::shared_ptr<AbstractMap> map,
                         std::shared_ptr<LightMap> light_map)
    : buffers_(save_file, constants::kDefaultClearTimeMSec),
      index_buffer_(QOpenGLBuffer::Type::IndexBuffer),
      map_(std::move(map)),
      light_map_(std::move(light_map)) {}

void GLMapDrawer::InitImpl() {
  GenerateIndexBuffer(&index_buffer_);
  LoadShader(&shader_);
  block_textures_.Init();
  backgrounds_.Init();
}

void GLMapDrawer::DrawMapWithCenterImpl(QPainter* painter, const QPointF& pos,
                                        const QRect& screen_coords) {
  Q_UNUSED(painter);
  auto* gl = GLFunctions::GetInstance();

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
  shader_.setUniformValue("global_sun", 1.0F);

  for (int32_t x = start.x(); x <= finish.x(); x += kMeshWidth) {
    for (int32_t y = start.y(); y <= finish.y(); y += kMeshHeight) {
      QPointF point = QPointF(x, y) - pos;
      shader_.setUniformValue("buffer_pos", point);
      auto* mesh = GetMesh(QPoint(x, y));
      mesh->bind();

      shader_.setUniformValue("player_pos", QPointF(pos.x(), pos.y() / 10.0));
      shader_.setUniformValue(
          "tex_height", static_cast<GLfloat>(BlockDrawer::kBackgroundHeight) /
                            BackgroundAtlas::kPixmapHeight);
      for (int i = 0, shift = 0; i < kAttribsCount; ++i) {
        shift += kBackgroundStrides[i];
        gl->glEnableVertexAttribArray(i);
        gl->glVertexAttribPointer(
            i, kAttribSizes[i], GL_FLOAT, GL_FALSE, sizeof(VertexData),
            reinterpret_cast<void*>(shift * sizeof(GLfloat)));
        shift += kAttribSizes[i];
      }

      backgrounds_.bind();
      gl->glDrawElements(GL_TRIANGLES, kElementsCount, GL_UNSIGNED_INT,
                         nullptr);
      backgrounds_.release();

      shader_.setUniformValue("tex_height",
                              static_cast<GLfloat>(constants::kBlockSz) /
                                  TextureAtlas::kTextureHeight);
      shader_.setUniformValue("player_pos", QPointF(0.0, 0.0));
      for (int i = 0, shift = 0; i < kAttribsCount; ++i) {
        shift += kBlockStrides[i];
        gl->glEnableVertexAttribArray(i);
        gl->glVertexAttribPointer(
            i, kAttribSizes[i], GL_FLOAT, GL_FALSE, sizeof(VertexData),
            reinterpret_cast<void*>(shift * sizeof(GLfloat)));
        shift += kAttribSizes[i];
      }

      block_textures_.bind();
      gl->glDrawElements(GL_TRIANGLES, kElementsCount, GL_UNSIGNED_INT,
                         nullptr);
      block_textures_.release();

      mesh->release();
    }
  }
  // We don't need to invoke glDisableVertexAttribArray in each loop iteration,
  // like glEnableVertexAttribArray. This function is just used to keep Qt
  // invariant, so need to be called only once in the end.
  for (int i = 0; i < kAttribsCount; ++i) {
    gl->glDisableVertexAttribArray(i);
  }
  index_buffer_.release();
  shader_.release();
}

void GLMapDrawer::UpdateBlockImpl(QPoint position) {
  auto buffer_pos = RoundToMeshPos(position);
  auto buffer = buffers_.Get(buffer_pos);
  if (!buffer) {
    return;
  }
  QOpenGLBuffer& casted = buffer.value();
  casted.bind();
  const QPoint mesh_position = position - buffer_pos;;
  auto data = GetBlockData(position, mesh_position);
  casted.write(
      (mesh_position.y() * kMeshWidth + mesh_position.x()) * sizeof(BlockData),
      &data, sizeof(BlockData));
  casted.release();
}

GLMapDrawer::VertexData GLMapDrawer::GenData(QPoint pos, double z,
                                             QPointF tex_coords,
                                             QPointF back_coords, Light light,
                                             double pos_shift) {
  return VertexData{
      static_cast<GLfloat>(pos.x()),
      static_cast<GLfloat>(pos.y()),
      static_cast<GLfloat>(z),
      static_cast<GLfloat>(kBackgroundZ),
      static_cast<GLfloat>(tex_coords.x()),
      static_cast<GLfloat>(tex_coords.y()),
      static_cast<GLfloat>(back_coords.x()),
      static_cast<GLfloat>(back_coords.y()),
      static_cast<GLfloat>(light.GetRed()) / Light::kMaxLight,
      static_cast<GLfloat>(light.GetGreen()) / Light::kMaxLight,
      static_cast<GLfloat>(light.GetBlue()) / Light::kMaxLight,
      static_cast<GLfloat>(light.GetSun()) / Light::kMaxLight,
      static_cast<GLfloat>(pos_shift),
  };
}

QPoint GLMapDrawer::RoundToMeshPos(QPoint p) {
  return QPoint(p.x() - utils::ArithmeticalMod(p.x(), kMeshWidth),
                p.y() - utils::ArithmeticalMod(p.y(), kMeshHeight));
}

GLMapDrawer::VertexData GLMapDrawer::Average(const VertexData& a,
                                             const VertexData& b,
                                             const VertexData& c,
                                             const VertexData& d) {
  constexpr int kDataSize = sizeof(VertexData) / sizeof(GLfloat);
  using ArrayData = std::array<GLfloat, kDataSize>;
  std::array<const ArrayData*, 4> data{reinterpret_cast<const ArrayData*>(&a),
                                       reinterpret_cast<const ArrayData*>(&b),
                                       reinterpret_cast<const ArrayData*>(&c),
                                       reinterpret_cast<const ArrayData*>(&d)};
  ArrayData result{};
  for (int i = 0; i < kDataSize; ++i) {
    for (int j = 0; j < 4; ++j) {
      result[i] += (*data[j])[i];
    }
    result[i] /= 4.0F;
  }
  return *reinterpret_cast<VertexData*>(&result);
}

QRect GLMapDrawer::GetDrawRegionImpl(QPoint center) const {
  QRect result(center.x() - kFieldOfView, center.y() - kFieldOfView,
               2 * kFieldOfView + 1, 2 * kFieldOfView + 1);
  return result;
}

void GLMapDrawer::GenerateIndexBuffer(QOpenGLBuffer* index_buffer) {
  std::array<GLuint, kElementsCount> data{};
  for (int y = 0; y < kMeshHeight; ++y) {
    for (int x = 0; x < kMeshWidth; ++x) {
      int i = y * kMeshWidth + x;
      // Generates order of drawing queue.
      // To draw a block it draws 4 triangles with common vertex
      // in the center of block.
      data[kElementsPerBlock * i + 0] = kVerticesPerBlock * i + 0;
      data[kElementsPerBlock * i + 1] = kVerticesPerBlock * i + 1;
      data[kElementsPerBlock * i + 2] = kVerticesPerBlock * i + 4;
      data[kElementsPerBlock * i + 3] = kVerticesPerBlock * i + 1;
      data[kElementsPerBlock * i + 4] = kVerticesPerBlock * i + 2;
      data[kElementsPerBlock * i + 5] = kVerticesPerBlock * i + 4;
      data[kElementsPerBlock * i + 6] = kVerticesPerBlock * i + 2;
      data[kElementsPerBlock * i + 7] = kVerticesPerBlock * i + 3;
      data[kElementsPerBlock * i + 8] = kVerticesPerBlock * i + 4;
      data[kElementsPerBlock * i + 9] = kVerticesPerBlock * i + 3;
      data[kElementsPerBlock * i + 10] = kVerticesPerBlock * i + 0;
      data[kElementsPerBlock * i + 11] = kVerticesPerBlock * i + 4;
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
  bool link_result = shader->link();
  assert(link_result);
  for (int i = 0; i < kAttribsCount; ++i) {
    shader->bindAttributeLocation(kAttribNames[i], i);
  }
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
          QPoint(buffer_pos.x() + x, buffer_pos.y() + y), QPoint(x, y));
    }
  }

  InitGLBuffer(buffer, data.data(), sizeof(data));
}

GLMapDrawer::BlockData GLMapDrawer::GetBlockData(QPoint world_pos,
                                                 QPoint mesh_pos) {
  Block block = map_->GetBlock(world_pos);
  double z_coord =
      map_->GetBlock(world_pos).IsVisible() ? kBlocksZ : kNotVisibleZ;
  double pos_shift =
      map_->GetBlock(world_pos).GetBackType() == Block::BackType::kOverworld
          ? -0.012
          : 0.0;
  BlockData data{};
  data.left_top = GenData(mesh_pos, z_coord, TextureAtlas::GetBlockTCLT(block),
                          BackgroundAtlas::GetBackgroundTCLT(block, world_pos),
                          light_map_->GetLightLT(world_pos), pos_shift);
  data.right_top = GenData(QPoint(mesh_pos.x() + 1, mesh_pos.y()), z_coord,
                           TextureAtlas::GetBlockTCRT(block),
                           BackgroundAtlas::GetBackgroundTCRT(block, world_pos),
                           light_map_->GetLightRT(world_pos), pos_shift);
  data.left_bottom =
      GenData(QPoint(mesh_pos.x(), mesh_pos.y() + 1), z_coord,
              TextureAtlas::GetBlockTCLB(block),
              BackgroundAtlas::GetBackgroundTCLB(block, world_pos),
              light_map_->GetLightLB(world_pos), pos_shift);
  data.right_bottom =
      GenData(QPoint(mesh_pos.x() + 1, mesh_pos.y() + 1), z_coord,
              TextureAtlas::GetBlockTCRB(block),
              BackgroundAtlas::GetBackgroundTCRB(block, world_pos),
              light_map_->GetLightRB(world_pos), pos_shift);
  data.center = Average(data.left_top, data.left_bottom, data.right_top,
                        data.right_bottom);
  return data;
}
