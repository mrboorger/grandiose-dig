#include "model/abstract_map.h"

#include "controller/controller.h"

bool AbstractMap::HitBlock(QPoint pos, int hit_power) {
  auto* block = GetBlockMutable(pos);
  if (block->DecreaseDurability(hit_power)) {
    Controller::GetInstance()->PickItemToPlayer(
        InventoryItem::GetDropItem(*block));
    SetBlock(pos, Block(Block::Type::kAir));
    return true;
  }
  return false;
}
