#include "view/light_map.h"

#include <QDebug>

void LightMap::UpdateLight(QPoint pos) {
  qDebug() << remove_queue_.size();
  remove_queue_.push(pos);
}

const Light& LightMap::GetLight(QPoint pos) const {
  return data_.at(pos);
}

void LightMap::CalculateRegion(const QRect& region) {
  for (auto i = data_.begin(); i != data_.end();) {
    if (!region.contains(i->first)) {
      i = data_.erase(i);
    } else {
      ++i;
    }
  }
  qDebug() << "Light map size:" << data_.size();
  for (int y = region.top(); y <= region.bottom(); ++y) {
    for (int x = region.left(); x <= region.right(); ++x) {
      QPoint pos(x, y);
      if (!data_.count(pos)) {
        data_[pos] = map_->GetBlock(pos).GetLuminosity();
        updated_.insert(pos);
        if (!data_[pos].IsDark()) {
          set_queue_.push(pos);
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
        set_queue_.push(neighbour);
      }
    }
    data_[pos] = GetLuminosity(pos);
    if (!data_[pos].IsDark()) {
      set_queue_.push(pos);
    }
    updated_.insert(pos);
  }
  while (!set_queue_.empty()) {
    QPoint pos = set_queue_.front();
    set_queue_.pop();
    if (!region.contains(pos)) {
      continue;
    }
    const Light& light = data_[pos];
    for (auto neighbour : utils::PointNeighbours(pos)) {
      if (!region.contains(neighbour)) {
        continue;
      }
      if (!map_->GetBlock(neighbour).IsOpaque() &&
          data_[neighbour].CanBeUpdated(light)) {
        data_[neighbour].Combine(light);
        updated_.insert(neighbour);
        set_queue_.push(neighbour);
      }
    }
  }
}

Light LightMap::GetLuminosity(QPoint pos) {
  Light light = map_->GetBlock(pos).GetLuminosity();
  if (pos.y() < map_->GroundLevel()) {
    light.SetSun(Light::kMaxLight);
  }
  return light;
}
