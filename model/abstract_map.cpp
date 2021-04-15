#include "abstract_map.h"

#include "controller/controller.h"

void AbstractMap::HitBlock(QPoint pos, int hit_power) {
  auto* block = GetChangeableBlock(pos);
  if (block->DecreaseDurability(hit_power)) {
    // TODO(mrboorger): make different items
    Controller::GetInstance()->PickItemToPlayer(
        InventoryItem::GetDropItem(*block));
    SetBlock(pos, Block(Block::Type::kAir));
  }
}
