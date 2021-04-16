#ifndef MODEL_DAMAGE_H_
#define MODEL_DAMAGE_H_

#include <QPointF>

class Damage {
 public:
  enum class Type { kMagic, kMob, kPlayer, kFall, kTypesCount };
  static constexpr int kTypesCount = static_cast<int>(Type::kTypesCount);

  Damage() = default;
  explicit Damage(QPointF source, Type type = Type::kMagic, int amount = 10);
  Damage(Type type, int amount);

  void GetSource(QPointF source) { source_ = source; }
  void GetType(Type type) { type_ = type; }
  void GetAmount(int amount) { amount_ = amount; }

  QPointF GetSource() const { return source_; }
  Type GetType() const { return type_; }
  int GetAmount() const { return amount_; }

 private:
  QPointF source_;
  Type type_;
  int amount_;
};

#endif  // MODEL_DAMAGE_H_
