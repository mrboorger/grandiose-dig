#ifndef MODEL_DAMAGE_H_
#define MODEL_DAMAGE_H_

#include <QPointF>

class Damage {
 public:
  enum class Type { kMagic, kMob, kPlayer, kFall, kTypesCount };
  static constexpr int kTypesCount = static_cast<int>(Type::kTypesCount);

  Damage() = default;

  Damage(QPointF source, Type type, int amount)
      : source_(source), type_(type), amount_(amount) {}

  Damage(Type type, int amount) : source_(0, 0), type_(type), amount_(amount) {}

  void SetSource(QPointF source) { source_ = source; }
  void SetType(Type type) { type_ = type; }
  void SetAmount(int amount) { amount_ = amount; }

  QPointF GetSource() const { return source_; }
  Type GetType() const { return type_; }
  int GetAmount() const { return amount_; }

 private:
  QPointF source_;
  Type type_;
  int amount_;
};

#endif  // MODEL_DAMAGE_H_
