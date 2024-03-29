#ifndef TYPES_H_
#define TYPES_H_

#include <QPoint>

struct MobParameters {
  double walk_acceleration_;
  double walk_max_speed_;
  double walk_air_acceleration_;
  double walk_max_air_acceleration_;
  double jump_speed_;
  QPoint jump_in_blocks_;
  QPointF size_;
  QPointF damage_acceleration_;
  int health_;
  int damage_;
  int walk_animation_;
  int walk_pictures_;
  int attack_animation_;
  int attack_pictures_;
};

enum class EffectProcessType { kForward, kInverse };

#endif  // TYPES_H_
