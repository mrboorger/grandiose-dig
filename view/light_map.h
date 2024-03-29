#ifndef VIEW_LIGHT_MAP_H_
#define VIEW_LIGHT_MAP_H_

#include <QRect>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <set>
#include <utility>

#include "model/abstract_map.h"
#include "model/constants.h"
#include "model/region_cache.h"
#include "utils.h"

class LightMap {
 public:
  explicit LightMap(const QString& save_file, std::shared_ptr<AbstractMap> map)
      : data_(Container(save_file, constants::kDefaultClearTimeMSec,
                        BufferConstructor(this))),
        map_(std::move(map)) {}

  void UpdateLight(QPoint pos);
  Light GetLight(QPoint pos);
  // Returns light at specific block corner
  // (Left/Right, Top/Bottom)
  Light GetLightLT(QPoint pos);
  Light GetLightLB(QPoint pos);
  Light GetLightRT(QPoint pos);
  Light GetLightRB(QPoint pos);
  void CalculateRegion(const QRect& region);

  std::set<QPoint, utils::QPointLexicographicalCompare> TakeUpdateList();

 private:
  static constexpr int kBufferWidth = 64;
  static constexpr int kBufferHeight = 64;
  using Buffer = std::array<Light, kBufferWidth * kBufferHeight>;
  class BufferConstructor {
   public:
    explicit BufferConstructor(LightMap* light_map)
        : light_map_(light_map) {}

    Buffer operator()(const QString& save_file, QPoint pos);

   private:
    LightMap* light_map_;
  };
  class BufferSaver {
   public:
    BufferSaver() = default;
    void operator()(const QString& save_file, const QPoint& pos,
                    const Buffer& buffer);
  };
  using Container =
      containers::RegionCache<Light, kBufferWidth, kBufferHeight, Buffer,
                              BufferConstructor, BufferSaver>;
  static constexpr int kUpdateDepth = 2;

  void SetPointUpdated(QPoint pos, int iteration = kUpdateDepth);

  Light GetLuminosity(QPoint pos) const;

  Container data_;
  std::queue<QPoint> invalidate_queue_;
  std::set<QPoint, utils::QPointLexicographicalCompare> updated_;
  std::shared_ptr<AbstractMap> map_;
  std::recursive_mutex mutex_;
};

#endif  // VIEW_LIGHT_MAP_H_
