#ifndef MODEL_MOB_H_
#define MODEL_MOB_H_

#include <QImage>
#include <memory>
#include <utility>

#include "model/moving_object.h"
#include "model/strategy.h"

class Mob : public MovingObject {
 public:
  enum class Type { kZombie, kZombieLord, kQuiox, kMagicQuiox, kTypesCount };
  static constexpr int kTypesCount = static_cast<int>(Type::kTypesCount);

  Mob(QPointF pos, Type type);

  int32_t GetId() const { return static_cast<int32_t>(type_); }
  Type GetMobType() const { return type_; }

  void MoveMob(double time);

  void SetStrategy(std::shared_ptr<AbstractStrategy> strategy) {
    strategy_ = std::move(strategy);
  }

  static QPointF GetMobSize(int id);

  void Read(const QJsonObject& json) override;
  void Write(QJsonObject* json) const override;

 private:
  MobState mob_state_;
  Type type_;
  std::shared_ptr<AbstractStrategy> strategy_;
};

#endif  // MODEL_MOB_H_
