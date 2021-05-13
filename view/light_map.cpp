#include "view/light_map.h"

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
  std::queue<QPoint> update_queue;
  std::set<QPoint, utils::QPointLexicographicalCompare> in_update_queue;
  auto insert_in_update_queue = [&update_queue, &in_update_queue](QPoint pos) {
    if (!in_update_queue.count(pos)) {
      in_update_queue.insert(pos);
      update_queue.push(pos);
    }
  };
  for (int y = region.top(); y <= region.bottom(); ++y) {
    for (int x = region.left(); x <= region.right(); ++x) {
      QPoint pos(x, y);
      if (!data_.count(pos)) {
        Light light = GetLuminosity(pos);
        if (!light.IsDark()) {
          SetPointUpdated(pos);
          data_[pos] = light;
          insert_in_update_queue(pos);
        } else {
          for (auto i : utils::PointNeighbours(pos)) {
            if (!GetLight(i).IsDark()) {
              insert_in_update_queue(i);
            }
          }
        }
      }
    }
  }
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
        insert_in_update_queue(neighbour);
      }
    }
    Light luminosity = GetLuminosity(pos);
    if (!luminosity.IsDark()) {
      data_[pos] = luminosity;
      insert_in_update_queue(pos);
    } else {
      data_.erase(pos);
    }
    SetPointUpdated(pos);
  }
  while (!update_queue.empty()) {
    QPoint pos = update_queue.front();
    update_queue.pop();
    in_update_queue.erase(pos);
    if (!region.contains(pos)) {
      continue;
    }
    Light light =
        map_->GetBlock(pos).IsOpaque() ? GetLuminosity(pos) : data_[pos];
    if (light.IsDark()) {
      continue;
    }
    for (auto neighbour : utils::PointNeighbours(pos)) {
      if (!region.contains(neighbour)) {
        continue;
      }
      if (GetLight(neighbour).CanBeUpdated(light)) {
        data_[neighbour].Combine(light);
        SetPointUpdated(neighbour);
        insert_in_update_queue(pos);
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

Light LightMap::GetLuminosity(QPoint pos) const {
  Block block = map_->GetBlock(pos);
  Light light = block.GetLuminosity();
  if (pos.y() < map_->GroundLevel() && !block.IsOpaque()) {
    light.SetSun(Light::kMaxLight);
  }
  return light;
}
