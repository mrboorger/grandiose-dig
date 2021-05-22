#ifndef ALL_CRAFT_RECIPES_H_
#define ALL_CRAFT_RECIPES_H_

#include "model/craft_recipe.h"

class AllCraftRecipes {
 public:
  AllCraftRecipes() { LoadRecipes(); };

  int Size() const { return craft_recipes_.size(); };

  const CraftRecipe& GetRecipe(int num) const { return craft_recipes_[num]; };

 private:
  void LoadRecipes();

  std::vector<CraftRecipe> craft_recipes_;
};

#endif  // ALL_CRAFT_RECIPES_H_
