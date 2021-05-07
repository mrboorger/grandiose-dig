#include "model/effects.h"

Effect::Effect(Type type, double time, double strength)
    : type_(type), time_(time) {
  SetStrength(strength);
}

void Effect::SetStrength(double strength) {
  strength_ = strength;
  strength_ = std::max(strength_, 1.0);
}
