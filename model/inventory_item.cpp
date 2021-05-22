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
const std::array<Block, Block::kTypesCount> kBlockTypeFromItem = {
    Block(Block::Type::kAir), Block(Block::Type::kDirt),
    Block(Block::Type::kGrass), Block(Block::Type::kStone),
    Block(Block::Type::kTorch)};

}  // namespace

bool InventoryItem::operator==(const InventoryItem& rhs) const {
  return type_ == rhs.type_;
}

int InventoryItem::ItemsLeft() const {
  return kMaximumCount[static_cast<int>(type_)] - count_;
}

InventoryItem InventoryItem::GetDropItem(Block block) {
  return kDropItemOfBlock[block.GetId()];
}

Block InventoryItem::GetBlockFromItem(InventoryItem item) {
  return kBlockTypeFromItem[item.GetIdOfBlock()];
}

bool InventoryItem::IsBlock() const {
  return (type_ >= Type::kBlockDirt && type_ <= Type::kBlockStone);
}

void InventoryItem::ChangeCount(int new_count) {
  count_ = new_count;
  if (count_ <= 0) {
    type_ = Type::kEmptyItem;
    count_ = 0;
  }
}
