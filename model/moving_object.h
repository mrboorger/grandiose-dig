#ifndef MODEL_MOVING_OBJECT_H_
#define MODEL_MOVING_OBJECT_H_

#include <memory>
#include <unordered_set>

#include "controller/controller_types.h"
#include "model/constants.h"
#include "model/map.h"
#include "model/move_vector.h"

class MovingObject {
 public:
  enum class State { kStay, kWalk, kJump };

  void SetWalkAcceleration(double speed);
  void SetWalkMaxSpeed(double speed);
  void SetWalkAirAcceleration(double speed);
  void SetWalkMaxAirAcceleration(double speed);
  void SetGravitySpeed(double speed);
  void SetJumpSpeed(double speed);

  double GetWalkAcceleration() const;
  double GetWalkMaxSpeed() const;
  double GetWalkAirAcceleration() const;
  double GetWalkMaxAirAcceleration() const;
  double GetGravitySpeed() const;
  double GetJumpSpeed() const;

  QPointF GetPosition() const;

  void Move(const std::unordered_set<ControllerTypes::Key>& pressed_keys);

 protected:
  explicit MovingObject(QPointF pos, QPointF size);

 private:
  void UpdateStay(const std::unordered_set<ControllerTypes::Key>& pressed_keys);
  void UpdateWalk(const std::unordered_set<ControllerTypes::Key>& pressed_keys);
  void UpdateJump(const std::unordered_set<ControllerTypes::Key>& pressed_keys);
  void UpdateState(
      const std::unordered_set<ControllerTypes::Key>& pressed_keys);
  void CheckCollisions(QPointF old_position);
  bool FindCollisionGround(QPointF old_position, double* ground_y,
                           const std::shared_ptr<const Map>& map) const;
  bool FindCollisionCeiling(QPointF old_position, double* ceiling_y,
                            const std::shared_ptr<const Map>& map) const;
  bool FindCollisionLeft(QPointF old_position, double* left_wall_x,
                         const std::shared_ptr<const Map>& map) const;
  bool FindCollisionRight(QPointF old_position, double* right_wall_x,
                          const std::shared_ptr<const Map>& map) const;
  MoveVector move_vector_ = MoveVector(0, 0, 0, 0);
  QPointF pos_;
  QPointF size_;
  State state_ = MovingObject::State::kStay;

  double walk_acceleration_;
  double walk_max_speed_;
  double walk_air_acceleration_;
  double walk_max_air_acceleration_;

  double gravity_speed_;
  double jump_speed_;

  int state_ticks_ = 0;

  bool pushes_ground_ = false;
  bool pushes_ceil_ = false;
  bool pushes_left_ = false;
  bool pushes_right_ = false;
};

#endif  // MODEL_MOVING_OBJECT_H_
