#include "view/light_map.h"

#include "utils.h"

void LightMap::UpdateLight(QPoint pos) {
  invalidate_queue_.push(pos);
}

Light LightMap::GetLight(QPoint pos) {
  auto res = data_.TryGetValue(pos);
  if (!res) {
    return Light();
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
    QPoint pos) {
  for (int32_t y = pos.y(); y < pos.y() + LightMap::kBufferHeight; ++y) {
    for (int32_t x = pos.x(); x < pos.x() + LightMap::kBufferWidth; ++x) {
      update_queue_->push(QPoint(x, y));
    }
  }
  return Buffer{};
}

void LightMap::SetPointUpdated(QPoint pos, int iteration) {
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
