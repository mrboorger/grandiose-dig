#include "model/chunk.h"

#include <QJsonArray>

void Chunk::FillWith(Block block) {
  for (int y = 0; y < kHeight; ++y) {
    for (int x = 0; x < kWidth; ++x) {
      SetBlock(QPoint(x, y), block);
    }
  }
}

void Chunk::Read(const QJsonObject& json) {
  QJsonArray items = json["blocks"].toArray();
  for (int index = 0; index < items.size(); ++index) {
    blocks_[index].Read(items[index].toObject());
  }
}

void Chunk::Write(QJsonObject& json) const {
  QJsonArray items;
  for (size_t index = 0; index < blocks_.size(); ++index) {
    QJsonObject item;
    blocks_[index].Write(item);
    items.append(item);
  }
  json["blocks"] = items;
}
