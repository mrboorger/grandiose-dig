#ifndef MODEL_ABSTRACT_MAP_H_
#define MODEL_ABSTRACT_MAP_H_

#include <QPoint>
#include <QRect>

#include "model/block.h"

class AbstractMap {
 public:
  AbstractMap() = default;
  AbstractMap(const AbstractMap&) = default;
  AbstractMap(AbstractMap&&) = default;

  virtual ~AbstractMap() = default;

  AbstractMap& operator=(const AbstractMap&) = default;
  AbstractMap& operator=(AbstractMap&&) = default;

  const Block& GetBlock(QPoint pos) { return *GetChangeableBlock(pos); }
  virtual void SetBlock(QPoint pos, Block block) = 0;

  virtual void CacheRegion(const QRect& region) { Q_UNUSED(region); }

  void HitBlock(QPoint pos, int hit_power);

 protected:
  virtual Block* GetChangeableBlock(QPoint pos) = 0;
};

#endif  // MODEL_ABSTRACT_MAP_H_
