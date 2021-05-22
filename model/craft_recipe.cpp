#include "model/craft_recipe.h"

#include "controller/controller.h"

CraftRecipe::CraftRecipe(InventoryItem resulting_item,
                         std::vector<InventoryItem> needed_items)
    : resulting_item_(std::move(resulting_item)),
      needed_items_(std::move(needed_items)) {}

CraftRecipe::CraftRecipe(CraftRecipe&& recipe) noexcept {
  resulting_item_ = recipe.resulting_item_;
  needed_items_ = std::move(recipe.needed_items_);
}

void CraftRecipe::TryCraft() const {
  Controller::GetInstance()->TryCraft(*this);
}
