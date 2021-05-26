#include "model/chunk_map.h"

#include <utility>

#include "model/constants.h"

ChunkMap::ChunkMap(const QString& save_file, AbstractRegionGenerator* generator)
    : nodes_(save_file, constants::kDefaultClearTimeMSec, GenChunk(generator)),
      generator_(generator) {}

Block* ChunkMap::GetBlockMutableImpl(QPoint pos) {
  return nodes_.GetMutableValue(pos);
}

void ChunkMap::SetBlockImpl(QPoint pos, Block block) {
  nodes_.SetValue(pos, block);
}

void ChunkMap::CacheRegionImpl(const QRect& region) {
  nodes_.MarkUsedOrInsert(region);
}

Chunk ChunkMap::GenChunk::operator()(const QString& save_file, QPoint pos) {
  return generator_->Generate(save_file, pos);
}

void ChunkMap::SaveChunk::operator()(const QString& save_file,
                                     const QPoint& pos, const Chunk& chunk) {
  QFile file(save_file + "chunk:" + QString::number(pos.x()) + ":" +
             QString::number(pos.y()));
  if (!file.open(QIODevice::WriteOnly)) {
    qWarning("Couldn't open save file.");
    return;
  }
  QJsonObject data;
  chunk.Write(&data);
  file.write(QCborValue::fromJsonValue(data).toCbor());
}
