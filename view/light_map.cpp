#include "view/light_map.h"

#include <QDebug>
#include "utils.h"

void LightMap::UpdateLight(QPoint pos) {
  remove_queue_.push(pos);
}

Light LightMap::GetLight(QPoint pos) const {
  auto res = data_.find(pos);
  if (res == data_.end()) {
    return Light();
  }
  return res->second;
}

Light LightMap::GetLightLT(QPoint pos) const {
  auto res = GetLight(pos);
  res.GetMax(GetLight(QPoint(pos.x(), pos.y() - 1)));
  res.GetMax(GetLight(QPoint(pos.x() - 1, pos.y())));
  res.GetMax(GetLight(QPoint(pos.x() - 1, pos.y() - 1)));
  return res;
}

Light LightMap::GetLightLB(QPoint pos) const {
  auto res = GetLight(pos);
  res.GetMax(GetLight(QPoint(pos.x(), pos.y() + 1)));
  res.GetMax(GetLight(QPoint(pos.x() - 1, pos.y())));
  res.GetMax(GetLight(QPoint(pos.x() - 1, pos.y() + 1)));
  return res;
}

Light LightMap::GetLightRT(QPoint pos) const {
  auto res = GetLight(pos);
  res.GetMax(GetLight(QPoint(pos.x(), pos.y() - 1)));
  res.GetMax(GetLight(QPoint(pos.x() + 1, pos.y())));
  res.GetMax(GetLight(QPoint(pos.x() + 1, pos.y() - 1)));
  return res;
}

Light LightMap::GetLightRB(QPoint pos) const {
  auto res = GetLight(pos);
  res.GetMax(GetLight(QPoint(pos.x(), pos.y() + 1)));
  res.GetMax(GetLight(QPoint(pos.x() + 1, pos.y())));
  res.GetMax(GetLight(QPoint(pos.x() + 1, pos.y() + 1)));
  return res;
}

void LightMap::CalculateRegion(const QRect& region) {
  for (auto i = data_.begin(); i != data_.end();) {
    if (!region.contains(i->first) || i->second.IsDark()) {
      i = data_.erase(i);
    } else {
      ++i;
    }
  }
  for (int y = region.top(); y <= region.bottom(); ++y) {
    for (int x = region.left(); x <= region.right(); ++x) {
      QPoint pos(x, y);
      if (!data_.count(pos)) {
        Light light = GetLuminosity(pos);
        if (!light.IsDark()) {
          SetPointUpdated(pos);
          data_[pos] = light;
          set_queue_.push(pos);
        }
      }
    }
  }
  qDebug() << "Light map size:" << data_.size();
  while (!remove_queue_.empty()) {
    QPoint pos = remove_queue_.front();
    remove_queue_.pop();
    if (!region.contains(pos)) {
      continue;
    }
    const Light& light = data_[pos];
    for (auto neighbour : utils::PointNeighbours(pos)) {
      if (!region.contains(neighbour)) {
        continue;
      }
      if (data_[neighbour].IsDepended(light)) {
        remove_queue_.push(neighbour);
      } else if (!data_[neighbour].IsDark()) {
        set_queue_.push(neighbour);
      }
    }
    data_[pos] = GetLuminosity(pos);
    if (!data_[pos].IsDark()) {
      set_queue_.push(pos);
    } else {
      data_.erase(pos);
    }
    SetPointUpdated(pos);
  }
  while (!set_queue_.empty()) {
    QPoint pos = set_queue_.front();
    set_queue_.pop();
    if (!region.contains(pos)) {
      continue;
    }
    const Light& light = data_[pos];
    bool is_opaque = map_->GetBlock(pos).IsOpaque();
    for (auto neighbour : utils::PointNeighbours(pos)) {
      if (!region.contains(neighbour)) {
        continue;
      }
      if (!is_opaque && GetLight(neighbour).CanBeUpdated(light)) {
        data_[neighbour].Combine(light);
        SetPointUpdated(neighbour);
        set_queue_.push(neighbour);
      }
    }
  }
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

Light LightMap::GetLuminosity(QPoint pos) {
  Block block = map_->GetBlock(pos);
  Light light = block.GetLuminosity();
  if (pos.y() < map_->GroundLevel() && !block.IsOpaque()) {
    light.SetSun(Light::kMaxLight);
  }
  return light;
}
