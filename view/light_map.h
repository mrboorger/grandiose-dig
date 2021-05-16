#ifndef VIEW_LIGHT_MAP_H_
#define VIEW_LIGHT_MAP_H_

#include <QRect>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <utility>

#include "model/abstract_map.h"
#include "model/buffered_clearable_cache.h"
#include "model/constants.h"
#include "utils.h"

class LightMap {
 public:
  explicit LightMap(std::shared_ptr<AbstractMap> map) : map_(std::move(map)) {}

  void UpdateLight(QPoint pos);
  Light GetLight(QPoint pos);
  Light GetLightLT(QPoint pos);
  Light GetLightLB(QPoint pos);
  Light GetLightRT(QPoint pos);
  Light GetLightRB(QPoint pos);
  void CalculateRegion(const QRect& region);

  std::set<QPoint, utils::QPointLexicographicalCompare>* UpdateList() {
    return &updated_;
  }

 private:
  static constexpr int kBufferWidth = 64;
  static constexpr int kBufferHeight = 64;
  using Buffer = std::array<Light, kBufferWidth * kBufferHeight>;
  class BufferConstructor {
   public:
    explicit BufferConstructor(std::queue<QPoint>* update_queue)
        : update_queue_(update_queue) {}

    Buffer operator()(QPoint pos);

   private:
    std::queue<QPoint>* update_queue_;
  };
  using Container =
      containers::BufferedClearableCache<Light, kBufferWidth, kBufferHeight,
                                         Buffer, BufferConstructor>;
  static constexpr int kUpdateDeep = 1;

  void SetPointUpdated(QPoint pos, int iteration = kUpdateDeep);

  Light GetLuminosity(QPoint pos) const;

  Container data_ = Container(constants::kDefaultClearTimeMSec,
                              BufferConstructor(&remove_queue_));
  std::queue<QPoint> remove_queue_;
  std::set<QPoint, utils::QPointLexicographicalCompare> updated_;
  std::shared_ptr<AbstractMap> map_;
};

#endif  // VIEW_LIGHT_MAP_H_
