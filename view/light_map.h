#ifndef VIEW_LIGHT_MAP_H_
#define VIEW_LIGHT_MAP_H_

#include <QRect>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <utility>

#include "model/abstract_map.h"
#include "utils.h"

class LightMap {
 public:
  explicit LightMap(std::shared_ptr<AbstractMap> map) : map_(std::move(map)) {}

  void UpdateLight(QPoint pos);
  const Light& GetLight(QPoint pos) const;
  void CalculateRegion(const QRect& region);

  std::set<QPoint, utils::QPointLexicographicalCompare>* UpdateList() {
    return &updated_;
  }

 private:
  Light GetLuminosity(QPoint pos);

  std::map<QPoint, Light, utils::QPointLexicographicalCompare> data_;
  std::queue<QPoint> set_queue_;
  std::queue<QPoint> remove_queue_;
  std::set<QPoint, utils::QPointLexicographicalCompare> updated_;
  std::shared_ptr<AbstractMap> map_;
};

#endif  // VIEW_LIGHT_MAP_H_
