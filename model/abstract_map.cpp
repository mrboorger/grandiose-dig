#include "abstract_map.h"

#include "controller/controller.h"

void AbstractMap::HitBlock(QPoint pos, int hit_power) {
  if (GetChangeableBlock(pos)->DecreaseDurability(hit_power)) {
    // TODO(mrboorger): change it
    Controller::GetInstance()->PickItemToPlayer(
        InventoryItem(InventoryItem::Type::kBlockGrass));
    SetBlock(pos, Block(Block::Type::kAir));
  }
}

Block* AbstractMap::GetChangeableBlock(QPoint pos) {
  // TODO(mrboorger): ?
  return const_cast<Block*>(&GetBlock(pos));
}
