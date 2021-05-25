#ifndef CONTROLLER_CONTROLLER_TYPES_H_
#define CONTROLLER_CONTROLLER_TYPES_H_

#include <QString>

namespace ControllerTypes {
enum class Key {
  kLeft,
  kRight,
  kJump,
  kShowInventory,
  kInventoryPrevRow,
  kInventoryNextRow,
  kInventory0,
  kInventory1,
  kInventory2,
  kInventory3,
  kInventory4,
  kInventory5,
  kInventory6,
  kInventory7,
  kInventory8,
  kInventory9,
  kExit,
  kUnused
};

static constexpr int kKeysCount = static_cast<int>(Key::kUnused);

}  // namespace ControllerTypes

#endif  // CONTROLLER_CONTROLLER_TYPES_H_
