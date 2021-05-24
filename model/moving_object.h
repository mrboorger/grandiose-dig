#ifndef MODEL_MOVING_OBJECT_H_
#define MODEL_MOVING_OBJECT_H_

#include <memory>
#include <unordered_set>
#include <vector>

#include "controller/controller_types.h"
#include "model/abstract_map.h"
#include "model/constants.h"
#include "model/damage.h"
#include "model/effects.h"
#include "model/move_vector.h"
#include "utils.h"

class MovingObject {
 public:
  enum class State { kStay, kWalk, kJump };
  enum class Type { kUndefined, kPlayer, kMob };

  MovingObject(const MovingObject&) = default;
  MovingObject(MovingObject&&) = default;
  virtual ~MovingObject() = default;
  MovingObject& operator=(const MovingObject&) = default;
  MovingObject& operator=(MovingObject&&) = default;

  void SetType(Type type) { type_ = type; }
  Type GetType() const { return type_; }

  void SetWalkAcceleration(double speed) { walk_acceleration_ = speed; }
  void SetWalkMaxSpeed(double speed) { walk_max_speed_ = speed; }
  void SetWalkAirAcceleration(double speed) { walk_air_acceleration_ = speed; }
  void SetWalkMaxAirAcceleration(double speed) {
    walk_max_air_acceleration_ = speed;
  }

  void SetGravitySpeed(double speed) { gravity_speed_ = speed; }
  void SetJumpSpeed(double speed) { jump_speed_ = speed; }

  void SetHealth(int health) { health_ = health; }
  void SetDamage(int damage) { damage_ = damage; }
  void SetDamageAcceleration(QPointF damage_acceleration) {
    damage_acceleration_ = damage_acceleration;
  }

  void SetParamaters(const MobParameters& parameters) {
    damage_ = parameters.damage_;
    damage_acceleration_ = parameters.damage_acceleration_;
    health_ = parameters.health_;
    jump_speed_ = parameters.jump_speed_;
    size_ = parameters.size_;
    walk_acceleration_ = parameters.walk_acceleration_;
    walk_air_acceleration_ = parameters.walk_air_acceleration_;
    walk_max_air_acceleration_ = parameters.walk_max_air_acceleration_;
    walk_max_speed_ = parameters.walk_max_speed_;
  }

  double GetWalkAcceleration() const { return walk_acceleration_; }
  double GetWalkMaxSpeed() const { return walk_max_speed_; }
  double GetWalkAirAcceleration() const { return walk_air_acceleration_; }
  double GetWalkMaxAirAcceleration() const {
    return walk_max_air_acceleration_;
  }

  double GetGravitySpeed() const { return gravity_speed_; }
  double GetJumpSpeed() const { return jump_speed_; }

  QPointF GetPosition() const { return pos_; }
  QPointF GetSize() const { return size_; }

  QPointF GetDamageAcceleration() const { return damage_acceleration_; }
  int GetHealth() const { return health_; }
  int GetDamage() const { return damage_; }

  virtual void Move(
      const std::unordered_set<ControllerTypes::Key>& pressed_keys,
      double time);

  static bool IsObjectCollision(QPointF lhs_pos, QPointF lhs_size,
                                QPointF rhs_pos, QPointF rhs_size);

  bool IsOnGround() const { return pushes_ground_; }
  bool IsOnCeil() const { return pushes_ceil_; }
  bool IsPushesLeft() const { return pushes_left_; }
  bool IsPushesRight() const { return pushes_right_; }

  void CheckFallDamage();
  bool RecentlyDamaged() const { return damage_time_ > constants::kEps; }
  double GetDamageTime() const { return damage_time_; }
  void DealDamage(const Damage& damage);

  bool IsDead() const;

  bool IsInBlock(QPoint block_pos) const;

  void SetDirection(utils::Direction direction) { direction_ = direction; }
  utils::Direction GetDirection() const { return direction_; }

  State GetState() const { return state_; }

  double GetStateTime() const { return state_time_; }

  void AddEffect(Effect effect);
  void DeleteEffect(Effect::Type type);

 protected:
  MovingObject(QPointF pos, QPointF size);
  void UpdateState(const std::unordered_set<ControllerTypes::Key>& pressed_keys,
                   double time);

 private:
  void UpdateStay(const std::unordered_set<ControllerTypes::Key>& pressed_keys,
                  double time);
  void UpdateWalk(const std::unordered_set<ControllerTypes::Key>& pressed_keys,
                  double time);
  void UpdateJump(const std::unordered_set<ControllerTypes::Key>& pressed_keys,
                  double time);
  void MakeMovement(QPointF old_position, double time);
  void CheckCollisions(QPointF old_position);
  bool FindCollisionGround(QPointF old_position, double* ground_y,
                           const std::shared_ptr<AbstractMap>& map) const;
  bool FindCollisionCeiling(QPointF old_position, double* ceiling_y,
                            const std::shared_ptr<AbstractMap>& map) const;
  bool FindCollisionLeft(QPointF old_position, double* left_wall_x,
                         const std::shared_ptr<AbstractMap>& map) const;
  bool FindCollisionRight(QPointF old_position, double* right_wall_x,
                          const std::shared_ptr<AbstractMap>& map) const;

  void DecEffects(double time);

  void ProcessEffect(Effect effect, EffectProcessType k);
  void ApplySingularEffect(Effect effect);
  void ApplyEffect(Effect effect) {
    ProcessEffect(effect, EffectProcessType::kForward);
  }
  void UnapplyEffect(Effect effect) {
    ProcessEffect(effect, EffectProcessType::kInverse);
  }
  void CheckSingularEffects();

  std::vector<Effect> effects_;

  MoveVector move_vector_ = MoveVector(0, 0, 0, 0);
  QPointF pos_;   // in blocks
  QPointF size_;  // in blocks
  State state_ = MovingObject::State::kStay;

  double walk_acceleration_ = constants::kPlayerWalkAcceleration;
  double walk_max_speed_ = constants::kPlayerWalkMaxSpeed;
  double walk_air_acceleration_ = constants::kPlayerWalkAirAcceleration;
  double walk_max_air_acceleration_ = constants::kPlayerWalkMaxAirAcceleration;

  double gravity_speed_ = constants::kPlayerGravitySpeed;
  double jump_speed_ = constants::kPlayerJumpSpeed;

  QPointF damage_acceleration_ = constants::kPlayerDamageAcceleration;

  Type type_ = Type::kUndefined;

  int health_ = constants::kPlayerHealth;
  int damage_ = constants::kPlayerDamage;

  double state_time_ = 0;
  double damage_time_ = 0;

  bool pushes_ground_ = false;
  bool pushes_ceil_ = false;
  bool pushes_left_ = false;
  bool pushes_right_ = false;

  utils::Direction direction_ = utils::Direction::kRight;
};

#endif  // MODEL_MOVING_OBJECT_H_
