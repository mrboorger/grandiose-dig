#ifndef VIEW_LIGHT_MAP_H_
#define VIEW_LIGHT_MAP_H_

#include <QRect>
#include <atomic>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <set>
#include <thread>
#include <utility>

#include "model/abstract_map.h"
#include "model/constants.h"
#include "model/region_cache.h"
#include "utils.h"

class LightMap {
 public:
  explicit LightMap(std::shared_ptr<AbstractMap> map)
      : map_(std::move(map)), thread_([this]() { CalculateRegionThread(); }) {}

  ~LightMap() {
    qDebug() << "?";
    thread_stop_ = true;
    thread_.join();
  }

  void UpdateLight(QPoint pos);
  Light GetLight(QPoint pos);
  // Returns light at specific block corner
  // (Left/Right, Top/Bottom)
  Light GetLightLT(QPoint pos);
  Light GetLightLB(QPoint pos);
  Light GetLightRT(QPoint pos);
  Light GetLightRB(QPoint pos);
  void CalculateRegion(const QRect& region);
  void CalculateRegionThread();

  const std::set<QPoint, utils::QPointLexicographicalCompare>& TakeUpdateList()
      const {
    return updated_;
  }

  void ClearUpdateList() { updated_.clear(); }

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
  using Container = containers::RegionCache<Light, kBufferWidth, kBufferHeight,
                                            Buffer, BufferConstructor>;
  static constexpr int kUpdateDepth = 2;

  void SetPointUpdated(QPoint pos, int iteration = kUpdateDepth);

  Light GetLuminosity(QPoint pos) const;

  Container data_ = Container(constants::kDefaultClearTimeMSec,
                              BufferConstructor(&invalidate_queue_));
  std::queue<QPoint> invalidate_queue_;
  std::set<QPoint, utils::QPointLexicographicalCompare> updated_;
  std::shared_ptr<AbstractMap> map_;
  std::thread thread_;
  std::mutex mutex_;
  bool thread_stop_ = false;
};

#endif  // VIEW_LIGHT_MAP_H_
