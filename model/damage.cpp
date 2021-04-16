#include "model/damage.h"

Damage::Damage(QPointF source, Type type, int amount)
    : source_(source), type_(type), amount_(amount) {}

Damage::Damage(Type type, int amount) : type_(type), amount_(amount) {}
