#include "all_craft_recipes.h"

void CraftRecipeCollection::LoadRecipes() {
  craft_recipes_.push_back(
      CraftRecipe(InventoryItem(InventoryItem::Type::kSpeedPotion, 1),
                  {
                      InventoryItem(InventoryItem::Type::kBlockShimondOre, 1),
                  }));
  craft_recipes_.push_back(
      CraftRecipe(InventoryItem(InventoryItem::Type::kStrengthPotion, 1),
                  {
                      InventoryItem(InventoryItem::Type::kBlockFiremondOre, 1),
                  }));
  craft_recipes_.push_back(
      CraftRecipe(InventoryItem(InventoryItem::Type::kBlockYellowLight, 1),
                  {InventoryItem(InventoryItem::Type::kBlockCoalOre, 1)}));
  craft_recipes_.push_back(
      CraftRecipe(InventoryItem(InventoryItem::Type::kBlockBlueLight, 1),
                  {InventoryItem(InventoryItem::Type::kBlockCoalOre, 1)}));
  craft_recipes_.push_back(
      CraftRecipe(InventoryItem(InventoryItem::Type::kBlockGreenLight, 1),
                  {InventoryItem(InventoryItem::Type::kBlockCoalOre, 1)}));
  craft_recipes_.push_back(
      CraftRecipe(InventoryItem(InventoryItem::Type::kBlockRedLight, 1),
                  {InventoryItem(InventoryItem::Type::kBlockCoalOre, 1)}));
  craft_recipes_.push_back(
      CraftRecipe(InventoryItem(InventoryItem::Type::kBlockBrickBlock, 1),
                  {InventoryItem(InventoryItem::Type::kBlockClayBlock, 1),
                   InventoryItem(InventoryItem::Type::kBlockSand, 1)}));
  craft_recipes_.push_back(
      CraftRecipe(InventoryItem(InventoryItem::Type::kBlockStoneBricks, 1),
                  {InventoryItem(InventoryItem::Type::kBlockStone, 2)}));
  craft_recipes_.push_back(
      CraftRecipe(InventoryItem(InventoryItem::Type::kBlockSandstone, 1),
                  {InventoryItem(InventoryItem::Type::kBlockSand, 4)}));
}
