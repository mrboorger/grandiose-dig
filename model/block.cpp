#include "model/block.h"

#include <algorithm>
#include <array>
#include <cassert>

Light Block::GetLuminosity() const {
  auto result = GetFrontCharacteristics(GetFrontId()).luminosity;
  if (!GetFrontCharacteristics(GetFrontId()).is_opaque) {
    result.UpdateMax(GetBackCharacteristics(GetBackId()).luminosity);
  }
  return result;
}

const Block::FrontCharacteristics& Block::GetFrontCharacteristics(int32_t id) {
  static std::array<FrontCharacteristics, kFrontTypesCount> characteristics;
  static std::array<bool, kFrontTypesCount> is_set{};
  static bool is_initialized = false;
  if (!is_initialized) {
    auto set = [](FrontType type, const FrontCharacteristics& chs) {
      characteristics[static_cast<int>(type)] = chs;
      is_set[static_cast<int>(type)] = true;
    };
    is_initialized = true;
    set(FrontType::kAir,
        FrontCharacteristics{Light(0, 0, 0, 0), 0, false, false});
    set(FrontType::kDirt, kDefaultBlockCharactestics);
    set(FrontType::kGrass, kDefaultBlockCharactestics);
    set(FrontType::kSnowyGrass, kDefaultBlockCharactestics);
    set(FrontType::kSand, kDefaultBlockCharactestics);
    set(FrontType::kSandstone, kDefaultBlockCharactestics);
    set(FrontType::kStone, {Light(0, 0, 0, 0), 400, true, true});
    set(FrontType::kCoalOre, {Light(0, 0, 0, 0), 300, true, true});
    set(FrontType::kIronOre, {Light(0, 0, 0, 0), 700, true, true});
    set(FrontType::kShimondOre,
        FrontCharacteristics{Light(27, 108, 127, 0), 3000, true, false});
    set(FrontType::kFiremondOre,
        FrontCharacteristics{Light(127, 36, 22, 0), 3000, true, false});
    set(FrontType::kStoneBricks, {Light(0, 0, 0, 0), 800, true, true});
    set(FrontType::kClayBlock, kDefaultBlockCharactestics);
    set(FrontType::kBrickBlock, {Light(0, 0, 0, 0), 1000, true, true});
    set(FrontType::kTechnical, kDefaultBlockCharactestics);
    set(FrontType::kRedLight,
        FrontCharacteristics{Light(255, 64, 32, 0), 500, true, false});
    set(FrontType::kYellowLight,
        FrontCharacteristics{Light(255, 255, 144, 0), 500, true, false});
    set(FrontType::kGreenLight,
        FrontCharacteristics{Light(144, 255, 144, 0), 500, true, false});
    set(FrontType::kBlueLight,
        FrontCharacteristics{Light(144, 255, 255, 0), 500, true, false});
  }
  return characteristics[id];
}

const Block::BackCharacteristics& Block::GetBackCharacteristics(int32_t id) {
  static std::array<BackCharacteristics, kBackTypesCount> characteristics;
  static std::array<bool, kBackTypesCount> is_set{};
  static bool is_initialized = false;
  if (!is_initialized) {
    auto set = [](BackType type, const BackCharacteristics& chs) {
      characteristics[static_cast<int>(type)] = chs;
      is_set[static_cast<int>(type)] = true;
    };
    is_initialized = true;
    set(BackType::kOverworld,
        BackCharacteristics{Light(0, 0, 0, Light::kMaxLight), false});
    set(BackType::kCave, BackCharacteristics{Light(0, 0, 0, 0), false});
  }
  return characteristics[id];
}

int Block::GetRemainingDurabilityPercentage() const {
  int default_durability = GetDefaultDurability();
  return (100 * durability_ + default_durability - 1) / default_durability;
}

bool Block::DecreaseDurability(int delta) {
  if (front_type_ != FrontType::kAir) {
    durability_ = std::max(0, durability_ - delta);
  }
  return durability_ == 0;
}

void Block::Read(const QJsonObject& json) {
  front_type_ = static_cast<FrontType>(json["front_type"].toInt());
  back_type_ = static_cast<BackType>(json["back_type"].toInt());
  durability_ = json["durability"].toDouble();
}

void Block::Write(QJsonObject* json) const {
  (*json)["front_type"] = static_cast<int>(front_type_);
  (*json)["back_type"] = static_cast<int>(back_type_);
  (*json)["durability"] = durability_;
}
