#include "view/light_map.h"

#include "utils.h"

void LightMap::UpdateLight(QPoint pos) {
  remove_queue_.push(pos);
}

Light LightMap::GetLight(QPoint pos) {
  auto res = data_.GetValueOpt(pos);
  if (!res) {
    return Light();
  }
  return res.value();
}

Light LightMap::GetLightLT(QPoint pos) {
  auto res = GetLight(pos);
  res.GetMax(GetLight(QPoint(pos.x(), pos.y() - 1)));
  res.GetMax(GetLight(QPoint(pos.x() - 1, pos.y())));
  res.GetMax(GetLight(QPoint(pos.x() - 1, pos.y() - 1)));
  return res;
}

Light LightMap::GetLightLB(QPoint pos) {
  auto res = GetLight(pos);
  res.GetMax(GetLight(QPoint(pos.x(), pos.y() + 1)));
  res.GetMax(GetLight(QPoint(pos.x() - 1, pos.y())));
  res.GetMax(GetLight(QPoint(pos.x() - 1, pos.y() + 1)));
  return res;
}

Light LightMap::GetLightRT(QPoint pos) {
  auto res = GetLight(pos);
  res.GetMax(GetLight(QPoint(pos.x(), pos.y() - 1)));
  res.GetMax(GetLight(QPoint(pos.x() + 1, pos.y())));
  res.GetMax(GetLight(QPoint(pos.x() + 1, pos.y() - 1)));
  return res;
}

Light LightMap::GetLightRB(QPoint pos) {
  auto res = GetLight(pos);
  res.GetMax(GetLight(QPoint(pos.x(), pos.y() + 1)));
  res.GetMax(GetLight(QPoint(pos.x() + 1, pos.y())));
  res.GetMax(GetLight(QPoint(pos.x() + 1, pos.y() + 1)));
  return res;
}

void LightMap::CalculateRegion(const QRect& region) {
  std::queue<QPoint> update_queue;
  data_.MarkUsedOrInsert(region);
  while (!remove_queue_.empty()) {
    QPoint pos = remove_queue_.front();
    remove_queue_.pop();
    const Light& light = data_.GetValue(pos);
    for (auto neighbour : utils::PointNeighbours(pos)) {
      if (!data_.GetValueOpt(neighbour)) {
        continue;
      }
      if (data_.GetValue(neighbour).IsDepended(light)) {
        remove_queue_.push(neighbour);
      } else if (!data_.GetValue(neighbour).IsDark()) {
        update_queue.push(neighbour);
      }
    }
    Light luminosity = GetLuminosity(pos);
    if (!luminosity.IsDark()) {
      data_.SetValue(pos, luminosity);
      update_queue.push(pos);
    }
    SetPointUpdated(pos);
  }
  while (!update_queue.empty()) {
    QPoint pos = update_queue.front();
    update_queue.pop();
    if (!data_.GetValueOpt(pos)) {
      continue;
    }
    Light light = map_->GetBlock(pos).IsOpaque() ? GetLuminosity(pos)
                                                 : data_.GetValue(pos);
    if (light.IsDark()) {
      continue;
    }
    for (auto neighbour : utils::PointNeighbours(pos)) {
      if (!data_.GetValueOpt(neighbour)) {
        continue;
      }
      if (data_.GetMutableValue(neighbour)->Combine(light)) {
        SetPointUpdated(neighbour);
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
  for (auto neighbour : utils::PointNeighbours(pos)) {
    updated_.insert(neighbour);
    if (iteration != 0) {
      SetPointUpdated(neighbour, iteration - 1);
    }
  }
}

Light LightMap::GetLuminosity(QPoint pos) const {
  Block block = map_->GetBlock(pos);
  Light light = block.GetLuminosity();
  if (pos.y() < map_->GroundLevel() && !block.IsOpaque()) {
    light.SetSun(Light::kMaxLight);
  }
  return light;
}