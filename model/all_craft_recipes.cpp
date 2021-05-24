#include "all_craft_recipes.h"

void CraftRecipeCollection::LoadRecipes() {
  // TODO(mrboorger) : it is a temporary code
  craft_recipes_.push_back(
      CraftRecipe(InventoryItem(InventoryItem::Type::kBlockGrass, 1),
                  {
                      InventoryItem(InventoryItem::Type::kBlockDirt, 5),
                  }));
  for (int i = 0; i < 35; ++i) {
    craft_recipes_.push_back(std::move(
        CraftRecipe(InventoryItem(InventoryItem::Type::kBlockDirt, 1),
                    {
                        InventoryItem(InventoryItem::Type::kBlockGrass, 5),
                    })));
  }
}
