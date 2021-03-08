#ifndef MODEL_BLOCK_H_
#define MODEL_BLOCK_H_

#include <cstdint>

class Block {
 public:
  enum class Type {
    kAir,
    kDirt,
    kStone,
    kTypesCount,
  };

  static constexpr int kTypesCount = static_cast<int>(Type::kTypesCount);

  explicit Block(Type type) : type_(type) {}

  static bool IsVisible(Type type) { return type != Type::kAir; }
  bool IsVisible() const { return IsVisible(type_); }

  Type GetType() const { return type_; }

  int32_t GetId() const { return static_cast<int32_t>(type_); }

 private:
  Type type_;
};

#endif  // MODEL_BLOCK_H_
