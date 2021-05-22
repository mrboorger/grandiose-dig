#include "model/map.h"

#include <QJsonArray>
#include <algorithm>
#include <utility>

void Map::SetBlock(QPoint pos, Block block) {
  blocks_[pos.y() * width_ + pos.x()] = block;
}

Block* Map::GetBlockMutable(QPoint pos) {
  return &blocks_[pos.y() * width_ + pos.x()];
}

Map::Map(int width, int height)
    : blocks_(width * height, Block(Block::Type::kAir)),
      width_(width),
      height_(height) {}

void Map::Read(const QJsonObject& json) {
  width_ = json["width"].toInt();
  height_ = json["height"].toInt();
  QJsonArray blocks = json["blocks"].toArray();
  blocks_.clear();
  blocks_.reserve(width_ * height_);
  for (int index = 0; index < blocks.size(); ++index) {
    blocks_.emplace_back(Block::Type::kAir);
    blocks_.back().Read(blocks[index].toObject());
  }
}

void Map::Write(QJsonObject& json) const {
  json["width"] = width_;
  json["height"] = height_;
  QJsonArray blocks;
  for (int index = 0; index < blocks.size(); ++index) {
    QJsonObject block;
    blocks_[index].Write(block);
    blocks.append(block);
  }
  json["blocks"] = blocks;
}
