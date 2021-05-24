#include "model/craft_recipe.h"

#include <utility>

#include "controller/controller.h"

CraftRecipe::CraftRecipe(InventoryItem resulting_item,
                         std::vector<InventoryItem> needed_items)
    : resulting_item_(std::move(resulting_item)),
      needed_items_(std::move(needed_items)) {}

CraftRecipe::CraftRecipe(CraftRecipe&& recipe) noexcept {
  resulting_item_ = recipe.resulting_item_;
  needed_items_ = std::move(recipe.needed_items_);
}

CraftRecipe& CraftRecipe::operator=(CraftRecipe&& recipe) noexcept {
  resulting_item_ = recipe.resulting_item_;
  needed_items_ = std::move(recipe.needed_items_);
  return *this;
}

void CraftRecipe::TryCraft() const {
  Controller::GetInstance()->TryCraft(*this);
}

const std::vector<InventoryItem>& CraftRecipe::GetNeededItems() const {
  return needed_items_;
}
