#ifndef MODEL_MOVING_OBJECT_H_
#define MODEL_MOVING_OBJECT_H_

#include <QDebug>
#include <QImage>
#include <cmath>
#include <memory>
#include <unordered_set>

#include "model/constants.h"
#include "model/map.h"
#include "model/move_vector.h"

class MovingObject {
 public:
  enum class State { kStay, kWalk, kJump };

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
  MovingObject(QPointF pos, QPointF size, double walk_acceleration,
               double walk_max_speed, double walk_air_acceleration,
               double walk_max_air_acceleration, double gravity_speed,
               double jump_speed, State state = State::kStay,
               int state_ticks = 0, MoveVector move_vector = {0, 0, 0, 0});

 private:
  void UpdateState(const std::unordered_set<int>& pressed_keys);
  void UpdatePhysics(QPointF old_position);
  bool HasCollisionGround(QPointF old_position, double* ground_y,
                          std::shared_ptr<const Map> map) const;
  bool HasCollisionCeiling(QPointF old_position, double* ceiling_y,
                           std::shared_ptr<const Map> map) const;
  bool HasCollisionLeft(QPointF old_position, double* left_wall_x,
                        std::shared_ptr<const Map> map) const;
  bool HasCollisionRight(QPointF old_position, double* right_wall_x,
                         std::shared_ptr<const Map> map) const;
  QPointF pos_;
  QPointF size_;
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
