#include "model/inventory_item.h"

#include <array>

namespace {

constexpr std::array<int, static_cast<int>(InventoryItem::Type::kTypesCount)>
    kMaximumCount = {0,   999, 999, 999, 999, 999, 999, 999, 999, 999, 999,
                     999, 999, 999, 999, 999, 999, 999, 999, 999, 4,   4};

const std::array<InventoryItem,
                 static_cast<int>(InventoryItem::Type::kTypesCount)>
    kDropItemOfBlock = {InventoryItem(InventoryItem::Type::kEmptyItem),
                        InventoryItem(InventoryItem::Type::kBlockDirt),
                        InventoryItem(InventoryItem::Type::kBlockGrass),
                        InventoryItem(InventoryItem::Type::kBlockStone),
                        InventoryItem(InventoryItem::Type::kBlockSnowyGrass),
                        InventoryItem(InventoryItem::Type::kBlockSand),
                        InventoryItem(InventoryItem::Type::kBlockSandstone),
                        InventoryItem(InventoryItem::Type::kBlockCoalOre),
                        InventoryItem(InventoryItem::Type::kBlockIronOre),
                        InventoryItem(InventoryItem::Type::kBlockShimondOre),
                        InventoryItem(InventoryItem::Type::kBlockFiremondOre),
                        InventoryItem(InventoryItem::Type::kBlockStoneBricks),
                        InventoryItem(InventoryItem::Type::kBlockClayBlock),
                        InventoryItem(InventoryItem::Type::kBlockBrickBlock),
                        InventoryItem(InventoryItem::Type::kBlockTechnical),
                        InventoryItem(InventoryItem::Type::kBlockRedLight),
                        InventoryItem(InventoryItem::Type::kBlockYellowLight),
                        InventoryItem(InventoryItem::Type::kBlockGreenLight),
                        InventoryItem(InventoryItem::Type::kBlockBlueLight)};

const std::array<Block::FrontType, Block::kFrontTypesCount> kBlockTypeFromItem =
    {Block::FrontType::kAir,         Block::FrontType::kDirt,
     Block::FrontType::kGrass,       Block::FrontType::kStone,
     Block::FrontType::kSnowyGrass,  Block::FrontType::kSand,
     Block::FrontType::kSandstone,   Block::FrontType::kCoalOre,
     Block::FrontType::kIronOre,     Block::FrontType::kShimondOre,
     Block::FrontType::kFiremondOre, Block::FrontType::kStoneBricks,
     Block::FrontType::kClayBlock,   Block::FrontType::kBrickBlock,
     Block::FrontType::kTechnical,   Block::FrontType::kRedLight,
     Block::FrontType::kYellowLight, Block::FrontType::kGreenLight,
     Block::FrontType::kBlueLight};

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

bool InventoryItem::IsPotion() const {
  return type_ == Type::kSpeedPotion || type_ == Type::kStrengthPotion;
}

void InventoryItem::ChangeCount(int new_count) {
  count_ = new_count;
  if (count_ <= 0) {
    type_ = Type::kEmptyItem;
    count_ = 0;
  }
}

void InventoryItem::Read(const QJsonObject& json) {
  type_ = static_cast<Type>(json["type"].toInt());
  count_ = json["count"].toInt();
}

void InventoryItem::Write(QJsonObject* json) const {
  (*json)["type"] = static_cast<int>(type_);
  (*json)["count"] = count_;
}

int32_t InventoryItem::GetIdOfBlock() const {
  return static_cast<int32_t>(type_) - static_cast<int32_t>(Type::kBlockMin);
}
