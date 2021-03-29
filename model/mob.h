#ifndef MODEL_MOB_H
#define MODEL_MOB_H

#include <QImage>

#include "model/moving_object.h"
#include "model/strategy.h"

class Mob : public MovingObject {
 public:
  Mob(QPointF pos, QPointF size);
  enum class Type { kMob, kTypesCount };

  int32_t GetId() const { return static_cast<int32_t>(type_); }

  static constexpr int kTypesCount = static_cast<int>(Type::kTypesCount);

  void MoveMob();

 private:
  AbstractStrategy* strategy_;
  Type type_;
  MobState mob_state_;
};

#endif  // MODEL_MOB_H
