#include "model/effects.h"

Effect::Effect(Type type, double time, double strength)
    : type_(type), time_(time) {
  SetStrength(strength);
}

void Effect::Read(const QJsonObject& json) {
  type_ = static_cast<Type>(json["type"].toInt());
  time_ = json["time"].toDouble();
  SetStrength(json["strength"].toDouble());
  is_active_ = json["is_active"].toBool();
}

void Effect::Write(QJsonObject* json) const {
  (*json)["type"] = static_cast<int>(type_);
  (*json)["time"] = time_;
  (*json)["strength"] = strength_;
  (*json)["is_active"] = is_active_;
}
