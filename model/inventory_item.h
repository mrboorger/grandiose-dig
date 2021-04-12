#ifndef MODEL_INVENTORY_ITEM_H_
#define MODEL_INVENTORY_ITEM_H_

#include <cstdint>

class InventoryItem {
 public:
  // TODO(mrboorger): change to Type
  enum class Type {
    kEmptyItem,
    kBlockGrass,
    kBlockDirt,
    kTypesCount,
  };

  static constexpr int kTypesCount = static_cast<int>(Type::kTypesCount);

  InventoryItem() : type_(Type::kEmptyItem), count_(0) {}

  explicit InventoryItem(Type type, int count = 1)
      : type_(type), count_(count) {}

  Type GetType() const { return type_; }
  int32_t GetId() const { return static_cast<int32_t>(type_); }
  int GetCount() const { return count_; }

  void ChangeCount(int new_count) { count_ = new_count; }

  int HowManyMoreItemsCanPut() const;

  // is usefull?
  static int GetMaximumCount(Type type);

 private:
  Type type_;
  int count_;
};

#endif  // MODEL_INVENTORY_ITEM_H_
