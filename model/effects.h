#ifndef MODEL_EFFECTS_H_
#define MODEL_EFFECTS_H_

#include <QJsonObject>
#include <algorithm>

class Effect {
 public:
  enum class Type {
    kSpeed,
    kSlowness,
    kStrength,
    kWeakness,
    kRegeneration,
    kPoison,
    kLightness,
    kHeaviness,
    kTypesCount
  };

  static constexpr int kTypesCount = static_cast<int>(Type::kTypesCount);

  explicit Effect(Type type, double time = 10, double strength = 1);

  void SetType(Type type) { type_ = type; }
  void DecTime(double time) { time_ = std::max(time_ - time, 0.0); }
  void SetTime(double time) { time_ = time; }
  void SetStrength(double strength) { strength_ = std::max(strength, 1.0); }
  void ActivateEffect() { is_active_ = true; }
  void DeactivateEffect() { is_active_ = false; }

  Type GetType() const { return type_; }
  double GetTime() const { return time_; }
  double GetStrength() const { return strength_; }
  bool IsActive() const { return is_active_; }

  void Read(const QJsonObject& json);
  void Write(QJsonObject* json) const;

 private:
  Type type_;
  double time_ = 10;
  double strength_ = 1;
  bool is_active_ = false;
};

#endif  // MODEL_EFFECTS_H_
