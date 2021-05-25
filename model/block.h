#ifndef MODEL_BLOCK_H_
#define MODEL_BLOCK_H_

#include <QJsonObject>
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

  bool IsVisible() const { return GetCharactistics(GetId()).is_visible; }
  Light GetLuminosity() const { return GetCharactistics(GetId()).luminosity; }
  bool IsOpaque() const { return GetCharactistics(GetId()).is_opaque; }

  int GetDefaultDurability() const {
    return GetCharactistics(GetId()).default_durability;
  }
  Type GetType() const { return type_; }
  int32_t GetId() const { return static_cast<int32_t>(type_); }

  // returns true, if block is broken
  bool DecreaseDurability(int delta);

  void Read(const QJsonObject& json);
  void Write(QJsonObject* json) const;

 private:
  struct Characteristics {
    Light luminosity;
    int default_durability;
    bool is_visible;
    bool is_opaque;
  };
  static constexpr Characteristics kDefaultBlockCharactestics{Light(0, 0, 0, 0),
                                                              200, true, true};

  static const Characteristics& GetCharactistics(int32_t id);

  Type type_;
  int durability_;
};

#endif  // MODEL_BLOCK_H_
