#ifndef MODEL_DAMAGE_H_
#define MODEL_DAMAGE_H_

#include <QPointF>

class Damage {
 public:
  enum class Type { kMagic, kMob, kPlayer, kFall, kTypesCount };
  static constexpr int kTypesCount = static_cast<int>(Type::kTypesCount);

  Damage() = default;

  Damage(QPointF source, Type type, int amount, QPointF acceleration)
      : source_(source),
        type_(type),
        amount_(amount),
        acceleration_(acceleration) {}

  Damage(Type type, int amount, QPointF acceleration)
      : source_(0, 0),
        type_(type),
        amount_(amount),
        acceleration_(acceleration) {}

  void SetSource(QPointF source) { source_ = source; }
  void SetType(Type type) { type_ = type; }
  void SetAmount(int amount) { amount_ = amount; }
  void SetDamageAcceleration(QPointF damage_acceleration) {
    acceleration_ = damage_acceleration;
  }

  QPointF GetSource() const { return source_; }
  Type GetType() const { return type_; }
  int GetAmount() const { return amount_; }
  QPointF GetDamageAcceleration() const { return acceleration_; }

 private:
  QPointF source_;
  Type type_;
  int amount_;
  QPointF acceleration_;
};

#endif  // MODEL_DAMAGE_H_
