#ifndef MODEL_EFFECTS_H_
#define MODEL_EFFECTS_H_

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
    kHeavyness,
    kTypesCount
  };

  static constexpr int kTypesCount = static_cast<int>(Type::kTypesCount);

  explicit Effect(Type type, double time = 10, double strength = 1);

  void DecTime(double time) { time_ = std::max(time_ - time, 0.0); }
  void SetTime(double time) { time_ = time; }
  void SetStrength(double strength) { strength_ = strength; }
  void ActivateEffect() { is_active_ = true; }
  void DeactivateEffect() { is_active_ = false; }

  Type GetType() const { return type_; }
  double GetTime() const { return time_; }
  double GetStrength() const { return strength_; }
  bool IsActive() const { return is_active_; }

 private:
  Type type_;
  double time_ = 10;
  double strength_ = 1;
  bool is_active_ = false;
};

#endif  // MODEL_EFFECTS_H_
