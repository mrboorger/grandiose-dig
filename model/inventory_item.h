#ifndef MODEL_INVENTORY_ITEM_H_
#define MODEL_INVENTORY_ITEM_H_

class InventoryItem {
 public:
  enum class ItemType {
    kEmptyItem,
    kBlockGrass,
    kBlockDirt,
    kTypesCount,
  };

  static constexpr int kTypesCount = static_cast<int>(ItemType::kTypesCount);

  InventoryItem() : type_(ItemType::kEmptyItem), count_(0) {}

  InventoryItem(ItemType type, int count) : type_(type), count_(count) {}

  ItemType GetItemType() const { return type_; }
  int GetId() const { return static_cast<int>(type_); }
  int GetCount() const { return count_; }

  void ChangeCount(int new_count) { count_ = new_count; }

  int HowManyMoreItemsCanPut() const;

  // is usefull?
  static int GetMaximumCount(ItemType type);

 private:
  ItemType type_;
  int count_;

};

#endif  // MODEL_INVENTORY_ITEM_H_
