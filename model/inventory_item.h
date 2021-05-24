#ifndef MODEL_INVENTORY_ITEM_H_
#define MODEL_INVENTORY_ITEM_H_

#include <cstdint>

#include "model/block.h"

class InventoryItem {
 public:
  // Must be arranged in groups
  enum class Type {
    kEmptyItem,
    kBlockMin = kEmptyItem,
    kBlockDirt,
    kBlockGrass,
    kBlockStone,
    kBlockMax,
    kTypesCount,
  };

  static constexpr int kTypesCount = static_cast<int>(Type::kTypesCount);

  InventoryItem() : type_(Type::kEmptyItem), count_(0) {}
  explicit InventoryItem(Type type, int count = 1)
      : type_(type), count_(count) {}
  InventoryItem(const InventoryItem&) = default;
  InventoryItem(InventoryItem&&) = default;

  InventoryItem& operator=(const InventoryItem&) = default;
  InventoryItem& operator=(InventoryItem&&) = default;

  bool operator==(const InventoryItem& rhs) const;

  bool IsEmpty() const { return type_ == Type::kEmptyItem; }

  Type GetType() const { return type_; }
  int32_t GetId() const { return static_cast<int32_t>(type_); }
  int32_t GetIdOfBlock() const;

  int GetCount() const { return count_; }

  void ChangeCount(int new_count);

  int ItemsLeft() const;

  static InventoryItem GetDropItem(Block block);
  static Block::FrontType GetBlockFromItem(InventoryItem item);

  bool IsBlock() const;

 private:
  Type type_;
  int count_;
};

#endif  // MODEL_INVENTORY_ITEM_H_
