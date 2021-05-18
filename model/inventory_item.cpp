#include "model/inventory_item.h"

#include <array>

namespace {

constexpr std::array<int, static_cast<int>(InventoryItem::Type::kTypesCount)>
    kMaximumCount = {0, 999, 999};
const std::array<InventoryItem,
                 static_cast<int>(InventoryItem::Type::kTypesCount)>
    kDropItemOfBlock = {InventoryItem(InventoryItem::Type::kEmptyItem),
                        InventoryItem(InventoryItem::Type::kBlockDirt),
                        InventoryItem(InventoryItem::Type::kBlockGrass),
                        InventoryItem(InventoryItem::Type::kBlockStone)};
}  // namespace

int InventoryItem::ItemsLeft() const {
  return kMaximumCount[static_cast<int>(type_)] - count_;
}

InventoryItem InventoryItem::GetDropItem(Block block) {
  return kDropItemOfBlock[block.GetId()];
}
