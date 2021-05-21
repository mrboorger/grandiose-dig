#ifndef MODEL_ABSTRACT_MAP_H_
#define MODEL_ABSTRACT_MAP_H_

#include <QPoint>
#include <QRect>
#include <mutex>

#include "model/block.h"

class AbstractMap {
 public:
  AbstractMap() = default;
  AbstractMap(const AbstractMap&) = default;
  AbstractMap(AbstractMap&&) = default;

  virtual ~AbstractMap() = default;

  AbstractMap& operator=(const AbstractMap&) = default;
  AbstractMap& operator=(AbstractMap&&) = default;

  const Block& GetBlock(QPoint pos) {
    const std::lock_guard<std::mutex> lock(mutex_);
    return *GetBlockMutable(pos);
  }
  virtual void SetBlock(QPoint pos, Block block) = 0;

  virtual int32_t GroundLevel() { return kDefaultGroundLevel; }

  virtual void CacheRegion(const QRect& region) { Q_UNUSED(region); }

  bool HitBlock(QPoint pos, int hit_power);

 protected:
  virtual Block* GetBlockMutable(QPoint pos) = 0;

 private:
  static constexpr int32_t kDefaultGroundLevel = 150;
  std::mutex mutex_;
};

#endif  // MODEL_ABSTRACT_MAP_H_
