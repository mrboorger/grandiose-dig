#ifndef MODEL_ABSTRACT_MAP_H_
#define MODEL_ABSTRACT_MAP_H_

#include <QJsonObject>
#include <QPoint>
#include <QRect>
#include <mutex>

#include "model/block.h"

class AbstractMap {
 public:
  AbstractMap() = default;
  AbstractMap(const AbstractMap&) = delete;
  AbstractMap(AbstractMap&&) = delete;

  virtual ~AbstractMap() = default;

  AbstractMap& operator=(const AbstractMap&) = delete;
  AbstractMap& operator=(AbstractMap&&) = delete;

  const Block& GetBlock(QPoint pos) { return *GetBlockMutable(pos); }
  void SetBlock(QPoint pos, Block block) {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    SetBlockImpl(pos, block);
  }

  int32_t GroundLevel() {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return GroundLevelImpl();
  }

  int32_t SkyLevel() {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return SkyLevelImpl();
  }

  void CacheRegion(const QRect& region) {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    CacheRegionImpl(region);
  }

  bool HitBlock(QPoint pos, int hit_power);

  virtual Block* GetBlockMutable(QPoint pos) {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return GetBlockMutableImpl(pos);
  }

 protected:
  virtual void SetBlockImpl(QPoint pos, Block block) = 0;
  virtual void CacheRegionImpl(const QRect& region) { Q_UNUSED(region); }
  virtual int32_t GroundLevelImpl() const { return kDefaultGroundLevel; }
  virtual int32_t SkyLevelImpl() const { return kDefaultSkyLevel; }
  virtual Block* GetBlockMutableImpl(QPoint pos) = 0;

 private:
  static constexpr int32_t kDefaultGroundLevel = 128;
  static constexpr int32_t kDefaultSkyLevel = 63;
  std::recursive_mutex mutex_;
};

#endif  // MODEL_ABSTRACT_MAP_H_
