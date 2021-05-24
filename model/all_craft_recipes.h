#ifndef MODEL_ALL_CRAFT_RECIPES_H_
#define MODEL_ALL_CRAFT_RECIPES_H_

#include <vector>

#include "model/craft_recipe.h"

class CraftRecipeCollection {
 public:
  CraftRecipeCollection() { LoadRecipes(); }

  int Size() const { return craft_recipes_.size(); }

  const CraftRecipe& GetRecipe(int num) const { return craft_recipes_[num]; }

 private:
  void LoadRecipes();

  std::vector<CraftRecipe> craft_recipes_;
};

#endif  // MODEL_ALL_CRAFT_RECIPES_H_
