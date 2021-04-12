#include "inventory_item.h"

#include <array>

namespace {
const std::array<int, static_cast<int>(InventoryItem::Type::kTypesCount)>
    maximum_count = {0, 999, 999};
}  // namespace

int InventoryItem::GetMaximumCount(InventoryItem::Type type) {
  return maximum_count[static_cast<int>(type)];
}

int InventoryItem::HowManyMoreItemsCanPut() const {
  return maximum_count[static_cast<int>(type_)] - count_;
}
