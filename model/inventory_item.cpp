#include "inventory_item.h"

#include <assert.h>

#include <array>

namespace {

constexpr std::array<int, static_cast<int>(InventoryItem::Type::kTypesCount)>
    maximum_count = {0, 999, 999};
}  // namespace

int InventoryItem::HowManyMoreItemsCanPut() const {
  return maximum_count[static_cast<int>(type_)] - count_;
}

InventoryItem InventoryItem::GetDropItem(Block block) {
  switch (block.GetType()) {
    case Block::Type::kDirt:
      return InventoryItem(InventoryItem::Type::kBlockDirt);
    case Block::Type::kGrass:
      return InventoryItem(InventoryItem::Type::kBlockGrass);
    case Block::Type::kStone:
      return InventoryItem(InventoryItem::Type::kBlockStone);
    default:
      assert(false);
  }
}
