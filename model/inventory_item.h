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

  InventoryItem() : type_(ItemType::kEmptyItem), count_(count_) {}

  InventoryItem(ItemType type, int count) : type_(type), count_(count) {}

  ItemType GetItemType() { return type_; }
  int GetCount() { return count_; }

  void ChangeCount(int new_count) { count_ = new_count; }

  int HowManyMoreItemsCanPut();

  static int GetMaximumCount(ItemType type);

 private:
  ItemType type_;
  int count_;

};

#endif  // MODEL_INVENTORY_ITEM_H_
