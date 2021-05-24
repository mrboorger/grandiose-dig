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
      CraftRecipe(InventoryItem(InventoryItem::Type::kBlockYellowLight, 9),
                  {InventoryItem(InventoryItem::Type::kBlockCoalOre, 8),
                   InventoryItem(InventoryItem::Type::kBlockFiremondOre, 1)}));
  craft_recipes_.push_back(
      CraftRecipe(InventoryItem(InventoryItem::Type::kBlockBlueLight, 9),
                  {InventoryItem(InventoryItem::Type::kBlockCoalOre, 8),
                   InventoryItem(InventoryItem::Type::kBlockShimondOre, 1)}));
}
