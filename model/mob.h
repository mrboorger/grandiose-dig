#ifndef MODEL_MOB_H_
#define MODEL_MOB_H_

#include <QImage>
#include <memory>

#include "model/moving_object.h"
#include "model/strategy.h"

class Mob : public MovingObject {
 public:
  enum class Type { kMob, kTypesCount };
  static constexpr int kTypesCount = static_cast<int>(Type::kTypesCount);

  Mob(QPointF pos, QPointF size);

  int32_t GetId() const { return static_cast<int32_t>(type_); }

  void MoveMob();

  void SetStrategy(std::shared_ptr<AbstractStrategy> strategy) {
    strategy_ = strategy;
  }

 private:
  MobState mob_state_;
  Type type_;
  std::shared_ptr<AbstractStrategy> strategy_;
};

#endif  // MODEL_MOB_H_
