#ifndef MODEL_MOVING_OBJECT_H_
#define MODEL_MOVING_OBJECT_H_

#include <memory>
#include <unordered_set>

#include "controller/controller_types.h"
#include "model/constants.h"
#include "model/abstract_map.h"
#include "model/move_vector.h"

class MovingObject {
 public:
  enum class State { kStay, kWalk, kJump };

  void SetWalkAcceleration(double speed) { walk_acceleration_ = speed; }
  void SetWalkMaxSpeed(double speed) { walk_max_speed_ = speed; }
  void SetWalkAirAcceleration(double speed) { walk_air_acceleration_ = speed; }
  void SetWalkMaxAirAcceleration(double speed) {
    walk_max_air_acceleration_ = speed;
  }

  void SetGravitySpeed(double speed) { gravity_speed_ = speed; }
  void SetJumpSpeed(double speed) { jump_speed_ = speed; }

  double GetWalkAcceleration() const { return walk_acceleration_; }
  double GetWalkMaxSpeed() const { return walk_max_speed_; }
  double GetWalkAirAcceleration() const { return walk_air_acceleration_; }
  double GetWalkMaxAirAcceleration() const {
    return walk_max_air_acceleration_;
  }

  double GetGravitySpeed() const { return gravity_speed_; }
  double GetJumpSpeed() const { return jump_speed_; }

  QPointF GetPosition() const { return pos_; }

  void Move(const std::unordered_set<ControllerTypes::Key>& pressed_keys);

 protected:
  MovingObject(QPointF pos, QPointF size);

 private:
  void UpdateStay(const std::unordered_set<ControllerTypes::Key>& pressed_keys);
  void UpdateWalk(const std::unordered_set<ControllerTypes::Key>& pressed_keys);
  void UpdateJump(const std::unordered_set<ControllerTypes::Key>& pressed_keys);
  void UpdateState(
      const std::unordered_set<ControllerTypes::Key>& pressed_keys);
  void CheckCollisions(QPointF old_position);
  bool FindCollisionGround(QPointF old_position, double* ground_y,
                           const std::shared_ptr<AbstractMap>& map) const;
  bool FindCollisionCeiling(QPointF old_position, double* ceiling_y,
                            const std::shared_ptr<AbstractMap>& map) const;
  bool FindCollisionLeft(QPointF old_position, double* left_wall_x,
                         const std::shared_ptr<AbstractMap>& map) const;
  bool FindCollisionRight(QPointF old_position, double* right_wall_x,
                          const std::shared_ptr<AbstractMap>& map) const;
  MoveVector move_vector_ = MoveVector(0, 0, 0, 0);
  QPointF pos_;
  QPointF size_;
  State state_ = MovingObject::State::kStay;

  double walk_acceleration_ = 0.01;
  double walk_max_speed_ = 0.1;
  double walk_air_acceleration_ = 0.01;
  double walk_max_air_acceleration_ = 0.1;

  double gravity_speed_ = 0.01;
  double jump_speed_ = -0.3;

  int state_ticks_ = 0;

  bool pushes_ground_ = false;
  bool pushes_ceil_ = false;
  bool pushes_left_ = false;
  bool pushes_right_ = false;
};

#endif  // MODEL_MOVING_OBJECT_H_
