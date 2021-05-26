#include "view/light_map.h"

#include <QCborMap>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <mutex>

#include "utils.h"

void LightMap::UpdateLight(QPoint pos) {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  invalidate_queue_.push(pos);
}

Light LightMap::GetLight(QPoint pos) {
  std::unique_lock<std::recursive_mutex> lock(mutex_, std::try_to_lock);
  auto res = data_.TryGetValue(pos);
  if (!res) {
    return map_->GetBlock(pos).GetLuminosity();
  }
  return res.value();
}

Light LightMap::GetLightLT(QPoint pos) {
  auto res = GetLight(pos);
  res.UpdateMax(GetLight(QPoint(pos.x(), pos.y() - 1)));
  res.UpdateMax(GetLight(QPoint(pos.x() - 1, pos.y())));
  res.UpdateMax(GetLight(QPoint(pos.x() - 1, pos.y() - 1)));
  return res;
}

Light LightMap::GetLightLB(QPoint pos) {
  auto res = GetLight(pos);
  res.UpdateMax(GetLight(QPoint(pos.x(), pos.y() + 1)));
  res.UpdateMax(GetLight(QPoint(pos.x() - 1, pos.y())));
  res.UpdateMax(GetLight(QPoint(pos.x() - 1, pos.y() + 1)));
  return res;
}

Light LightMap::GetLightRT(QPoint pos) {
  auto res = GetLight(pos);
  res.UpdateMax(GetLight(QPoint(pos.x(), pos.y() - 1)));
  res.UpdateMax(GetLight(QPoint(pos.x() + 1, pos.y())));
  res.UpdateMax(GetLight(QPoint(pos.x() + 1, pos.y() - 1)));
  return res;
}

Light LightMap::GetLightRB(QPoint pos) {
  auto res = GetLight(pos);
  res.UpdateMax(GetLight(QPoint(pos.x(), pos.y() + 1)));
  res.UpdateMax(GetLight(QPoint(pos.x() + 1, pos.y())));
  res.UpdateMax(GetLight(QPoint(pos.x() + 1, pos.y() + 1)));
  return res;
}

void LightMap::CalculateRegion(const QRect& region) {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  std::queue<QPoint> update_queue;
  std::set<QPoint, utils::QPointLexicographicalCompare> removed;
  data_.MarkUsedOrInsert(region);
  while (!invalidate_queue_.empty()) {
    QPoint pos = invalidate_queue_.front();
    removed.insert(pos);
    invalidate_queue_.pop();
    const Light& light = data_.GetValue(pos);
    for (auto neighbour : utils::NeighbourPoints(pos)) {
      if (!data_.TryGetValue(neighbour)) {
        continue;
      }
      if (data_.GetValue(neighbour).IsDepenantOn(light)) {
        if (removed.count(neighbour) == 0) {
          invalidate_queue_.push(neighbour);
          removed.insert(neighbour);
        }
      } else if (!data_.GetValue(neighbour).IsDark()) {
        update_queue.push(neighbour);
      }
    }
    Light luminosity = GetLuminosity(pos);
    if (!luminosity.IsDark()) {
      update_queue.push(pos);
    }
    data_.SetValue(pos, luminosity);
    SetPointUpdated(pos);
  }
  while (!update_queue.empty()) {
    QPoint pos = update_queue.front();
    update_queue.pop();
    if (!data_.TryGetValue(pos)) {
      continue;
    }
    Light light = map_->GetBlock(pos).IsOpaque() ? GetLuminosity(pos)
                                                 : data_.GetValue(pos);
    if (light.IsDark()) {
      continue;
    }
    for (auto neighbour : utils::NeighbourPoints(pos)) {
      if (!data_.TryGetValue(neighbour)) {
        continue;
      }
      if (data_.GetMutableValue(neighbour)->Combine(light)) {
        SetPointUpdated(neighbour);
        update_queue.push(neighbour);
      }
    }
  }
}

LightMap::Buffer LightMap::BufferConstructor::operator()(
    const QString& save_file, QPoint pos) {
  QFile file(save_file + "chunk:" + QString::number(pos.x()) + ":" +
             QString::number(pos.y()));
  if (!file.exists()) {
    for (int32_t y = pos.y(); y < pos.y() + LightMap::kBufferHeight; ++y) {
      for (int32_t x = pos.x(); x < pos.x() + LightMap::kBufferWidth; ++x) {
        light_map_->UpdateLight(QPoint(x, y));
      }
    }
    return Buffer{};
  }
  file.open(QIODevice::ReadOnly);
  QByteArray save_data = file.readAll();
  QJsonDocument data(
      QJsonDocument(QCborValue::fromCbor(save_data).toMap().toJsonObject()));
  Buffer buffer;
  QJsonArray arr = data["buffer"].toArray();
  for (int index = 0; index < kBufferHeight * kBufferWidth; ++index) {
    QJsonObject light = arr[index].toObject();
    buffer[index].Read(light);
    light_map_->SetPointUpdated(
        pos + QPoint(index % kBufferWidth, index / kBufferWidth));
  }
  return buffer;
}

void LightMap::BufferSaver::operator()(const QString& save_file,
                                       const QPoint& pos,
                                       const Buffer& buffer) {
  QFile file(save_file + "chunk:" + QString::number(pos.x()) + ":" +
             QString::number(pos.y()));
  if (!file.open(QIODevice::WriteOnly)) {
    qWarning("Couldn't open save file.");
    return;
  }
  QJsonObject data;
  QJsonArray arr;
  for (int index = 0; index < kBufferHeight * kBufferWidth; ++index) {
    QJsonObject light;
    buffer[index].Write(&light);
    arr.append(light);
  }
  data["buffer"] = arr;
  file.write(QCborValue::fromJsonValue(data).toCbor());
}

void LightMap::SetPointUpdated(QPoint pos, int iteration) {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  updated_.insert(pos);
  for (auto neighbour : utils::NeighbourPoints(pos)) {
    updated_.insert(neighbour);
    if (iteration != 0) {
      SetPointUpdated(neighbour, iteration - 1);
    }
  }
}

Light LightMap::GetLuminosity(QPoint pos) const {
  return map_->GetBlock(pos).GetLuminosity();
}
std::set<QPoint, utils::QPointLexicographicalCompare>

LightMap::TakeUpdateList() {
  std::set<QPoint, utils::QPointLexicographicalCompare> result;
  std::unique_lock<std::recursive_mutex> lock(mutex_, std::try_to_lock);
  if (lock.owns_lock()) {
    result.swap(updated_);
  }
  return result;
}
