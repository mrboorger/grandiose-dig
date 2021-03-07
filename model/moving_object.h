#ifndef MODEL_MOVING_OBJECT_H_
#define MODEL_MOVING_OBJECT_H_

#include <QDebug>
#include <QImage>
#include <memory>
#include <unordered_set>

#include "model/map.h"
#include "model/move_vector.h"

class MovingObject {
 public:
  enum class State { kStay, kWalk, kJump };

  static constexpr double kAbsoluteMaxSpeedX = 50;
  static constexpr double kAbsoluteMaxSpeedY = 50;

  void SetWalkAcceleration(double speed);
  void SetWalkMaxSpeed(double speed);
  void SetGravitySpeed(double speed);
  void SetJumpSpeed(double speed);

  double GetWalkAcceleration() const;
  double GetWalkMaxSpeed() const;
  double GetGravitySpeed() const;
  double GetJumpSpeed() const;

  QPointF GetPosition() const;

  void Move(const std::unordered_set<int>& pressed_keys);

 protected:
  MovingObject(QPointF pos, double size_x, double size_y,
               double walk_acceleration, double walk_max_speed,
               double walk_air_acceleration, double walk_max_air_acceleration,
               double gravity_speed, double jump_speed,
               State state = State::kStay, int state_ticks = 0,
               MoveVector move_vector = {{0, 0}, {0, 0}});

 private:
  void UpdateState(const std::unordered_set<int>& pressed_keys);
  void UpdatePhysics(QPointF old_position);
  QPointF pos_;
  // TODO(Wind-Eagle): make QPointF
  double size_x_;
  double size_y_;
  double walk_acceleration_;
  double walk_max_speed_;
  double walk_air_acceleration_;
  double walk_max_air_acceleration_;
  double gravity_speed_;
  double jump_speed_;
  State state_;
  int state_ticks_;
  MoveVector move_vector_;
  bool pushes_ground_;
  bool pushes_ceil_;
  bool pushes_right_;
  bool pushes_left_;
};

#endif  // MODEL_MOVING_OBJECT_H_
