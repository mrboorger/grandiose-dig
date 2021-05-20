#include "model/block.h"

#include <algorithm>
#include <array>
#include <cassert>

const Block::Characteristics& Block::GetCharactistics(int32_t id) {
  static std::array<Characteristics, kTypesCount> characteristics;
  static std::array<bool, kTypesCount> is_set{};
  static bool is_initialized = false;
  if (!is_initialized) {
    auto set = [](Type type, const Characteristics& chs) {
      characteristics[static_cast<int>(type)] = chs;
      is_set[static_cast<int>(type)] = true;
    };
    is_initialized = true;
    set(Type::kAir, Characteristics{Light(0, 0, 0, 0), 0, false, false});
    set(Type::kDirt, kDefaultBlockCharactestics);
    set(Type::kGrass, kDefaultBlockCharactestics);
    set(Type::kSnowyGrass, kDefaultBlockCharactestics);
    set(Type::kSand, kDefaultBlockCharactestics);
    set(Type::kSandstone, kDefaultBlockCharactestics);
    set(Type::kStone, kDefaultBlockCharactestics);
    set(Type::kCoalOre, kDefaultBlockCharactestics);
    set(Type::kIronOre, kDefaultBlockCharactestics);
    set(Type::kTechnical, kDefaultBlockCharactestics);
    set(Type::kTorch,
        Characteristics{Light(120, 120, 120, 0), 1, false, false});
  }
  assert(is_set[id]);
  return characteristics[id];
}

bool Block::DecreaseDurability(int delta) {
  if (type_ != Type::kAir) {
    durability_ = std::max(0, durability_ - delta);
  }
  return durability_ == 0;
}
