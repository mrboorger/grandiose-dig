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

const std::array<Block::FrontType, Block::kFrontTypesCount> kBlockTypeFromItem =
    {Block::FrontType::kAir, Block::FrontType::kDirt, Block::FrontType::kGrass,
     Block::FrontType::kStone, Block::FrontType::kTorch};

}  // namespace

bool InventoryItem::operator==(const InventoryItem& rhs) const {
  return type_ == rhs.type_;
}

int InventoryItem::ItemsLeft() const {
  return kMaximumCount[static_cast<int>(type_)] - count_;
}

InventoryItem InventoryItem::GetDropItem(Block block) {
  return kDropItemOfBlock[block.GetFrontId()];
}

Block::FrontType InventoryItem::GetBlockFromItem(InventoryItem item) {
  return kBlockTypeFromItem[item.GetIdOfBlock()];
}

bool InventoryItem::IsBlock() const {
  return (type_ > Type::kBlockMin && type_ < Type::kBlockMax);
}

void InventoryItem::ChangeCount(int new_count) {
  count_ = new_count;
  if (count_ <= 0) {
    type_ = Type::kEmptyItem;
    count_ = 0;
  }
}

int32_t InventoryItem::GetIdOfBlock() const {
  return static_cast<int32_t>(type_) - static_cast<int32_t>(Type::kBlockMin);
}
