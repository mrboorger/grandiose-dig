#include "model/effects.h"

Effect::Effect(Type type, double time, double strength)
    : type_(type), time_(time) {
  SetStrength(strength);
}
