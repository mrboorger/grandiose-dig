#ifndef MODEL_BLOCK_H_
#define MODEL_BLOCK_H_

#include <cstdint>

#include "view/light.h"

class Block {
 public:
  enum class Type {
    kFirst,
    kAir = kFirst,
    kDirt,
    kGrass,
    kStone,
    kTorch,
    kTypesCount,
  };

  static constexpr int kFirstType = static_cast<int>(Type::kFirst);
  static constexpr int kTypesCount = static_cast<int>(Type::kTypesCount);

  explicit Block(Type type)
      : type_(type), durability_(GetDefaultDurability()) {}

  bool IsAir() const { return type_ == Type::kAir; }

  bool IsVisible() const { return GetCharacteristics(GetId()).is_visible; }
  Light GetLuminosity() const { return GetCharacteristics(GetId()).luminosity; }
  bool IsOpaque() const { return GetCharacteristics(GetId()).is_opaque; }
  int GetDefaultDurability() const {
    return GetCharacteristics(GetId()).default_durability;
  }

  Type GetType() const { return type_; }
  int32_t GetId() const { return static_cast<int32_t>(type_); }
  int GetRemainingDurabilityPercentage() const;

  // returns true, if block is broken
  bool DecreaseDurability(int delta);

 private:
  struct Characteristics {
    Light luminosity;
    int default_durability;
    bool is_visible;
    bool is_opaque;
  };
  static constexpr Characteristics kDefaultBlockCharactestics{Light(0, 0, 0, 0),
                                                              200, true, true};

  static const Characteristics& GetCharacteristics(int32_t id);

  Type type_;
  int durability_;
};

#endif  // MODEL_BLOCK_H_
