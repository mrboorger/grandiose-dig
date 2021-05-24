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
    set(FrontType::kStone, kDefaultBlockCharactestics);
    set(FrontType::kCoalOre, kDefaultBlockCharactestics);
    set(FrontType::kIronOre, kDefaultBlockCharactestics);
    set(FrontType::kShimondOre,
        FrontCharacteristics{Light(27, 108, 127, 0), 1, true, false});
    set(FrontType::kFiremondOre,
        FrontCharacteristics{Light(127, 36, 22, 0), 1, true, false});
    set(FrontType::kTechnical, kDefaultBlockCharactestics);
    set(FrontType::kTorch,
        FrontCharacteristics{Light(120, 120, 120, 0), 1, true, false});
  }
  assert(is_set[id]);
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
  assert(is_set[id]);
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
