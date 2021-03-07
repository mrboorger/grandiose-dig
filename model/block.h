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

  explicit Block(Type type);

  bool IsVisible() const;
  static bool IsVisible(Type type);

  Type GetType() const;

  int32_t GetId() const;

 private:
  Type type_;
};

#endif  // MODEL_BLOCK_H_
