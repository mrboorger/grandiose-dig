#ifndef MODEL_BLOCK_H_
#define MODEL_BLOCK_H_

#include <cstdint>

class Block {
 public:
  enum class Type {
    kAir,
    kDirt,
    kGrass,
    kStone,
    kTypesCount,
  };

  static constexpr int kTypesCount = static_cast<int>(Type::kTypesCount);

  explicit Block(Type type) : type_(type) {}

  static bool IsVisible(Type type) { return type != Type::kAir; }
  bool IsVisible() const { return IsVisible(type_); }
  bool IsAir() const { return type_ == Type::kAir; };

  Type GetType() const { return type_; }

  int32_t GetId() const { return static_cast<int32_t>(type_); }

  // returns true, if block is broken
  bool DecreaseDurability(int delta);

 private:
  Type type_;
  // TODO(mrboorger): Make different durability_ of the blocks
  int durability_ = 5;
};

#endif  // MODEL_BLOCK_H_
