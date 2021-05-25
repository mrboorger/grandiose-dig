#ifndef MODEL_BLOCK_H_
#define MODEL_BLOCK_H_

#include <cstdint>

#include "view/light.h"

class Block {
 public:
  enum class FrontType {
    kFirst,
    kAir = kFirst,
    kDirt,
    kGrass,
    kStone,
    kSnowyGrass,
    kSand,
    kSandstone,
    kCoalOre,
    kIronOre,
    kShimondOre,
    kFiremondOre,
    kTechnical,
    kYellowLight,
    kBlueLight,
    kTypesCount,
  };

  enum class BackType {
    kFirst,
    kOverworld = kFirst,
    kCave,
    kTypesCount,
  };

  static constexpr int kFirstFrontType = static_cast<int>(FrontType::kFirst);
  static constexpr int kFrontTypesCount =
      static_cast<int>(FrontType::kTypesCount);
  static constexpr int kFirstBackType = static_cast<int>(BackType::kFirst);
  static constexpr int kBackTypesCount =
      static_cast<int>(BackType::kTypesCount);

  explicit Block(FrontType front_type)
      : front_type_(front_type),
        back_type_(front_type == FrontType::kAir ? BackType::kOverworld
                                                 : BackType::kCave),
        durability_(GetDefaultDurability()) {}
  Block(FrontType front_type, BackType back_type)
      : front_type_(front_type),
        back_type_(back_type),
        durability_(GetDefaultDurability()) {}

  bool IsVisible() const {
    return GetFrontCharacteristics(GetFrontId()).is_visible;
  }

  Light GetLuminosity() const;

  bool IsOpaque() const {
    return GetFrontCharacteristics(GetFrontId()).is_opaque ||
           GetBackCharacteristics(GetBackId()).is_opaque;
  }

  int GetDefaultDurability() const {
    return GetFrontCharacteristics(GetFrontId()).default_durability;
  }

  FrontType GetFrontType() const { return front_type_; }
  BackType GetBackType() const { return back_type_; }

  int32_t GetFrontId() const { return static_cast<int32_t>(front_type_); }
  int32_t GetBackId() const { return static_cast<int32_t>(back_type_); }

  void SetFrontType(FrontType front_type) { front_type_ = front_type; }
  void SetBackType(BackType back_type) { back_type_ = back_type; }
  bool IsAir() const { return front_type_ == FrontType::kAir; }
  int GetRemainingDurabilityPercentage() const;

  // returns true, if block is broken
  bool DecreaseDurability(int delta);

 private:
  struct FrontCharacteristics {
    Light luminosity;
    int default_durability;
    bool is_visible;
    bool is_opaque;
  };

  struct BackCharacteristics {
    Light luminosity;
    bool is_opaque;
  };

  static constexpr FrontCharacteristics kDefaultBlockCharactestics{
      Light(0, 0, 0, 0), 200, true, true};

  static const FrontCharacteristics& GetFrontCharacteristics(int32_t id);
  static const BackCharacteristics& GetBackCharacteristics(int32_t id);

  FrontType front_type_;
  BackType back_type_;
  int durability_;
};

#endif  // MODEL_BLOCK_H_
