#include "model/model.h"

#include <ctime>
#include <random>
#include <vector>

#include "utils.h"

Model* Model::GetInstance() {
  static Model model;
  return &model;
}

void Model::MoveObjects(
    const std::unordered_set<ControllerTypes::Key>& pressed_keys, double time) {
  // TODO(Wind-Eagle): this is temporary code.
  if (player_->IsDead()) {
    exit(0);
  }
  for (auto i = mobs_.begin(), last = mobs_.end(); i != last;) {
    if ((*i)->IsDead()) {
      i = mobs_.erase(i);
    } else {
      i++;
    }
  }

  player_->Move(pressed_keys, time);

  static std::uniform_real_distribution<double> distrib(0.0, 1.0);
  for (auto mob : mobs_) {
    mob->MoveMob(time);
    if (!mob->RecentlyDamaged() &&
        distrib(utils::random) < constants::kMobSoundChance) {
      emit MobSound(mob.get());
    }
  }
}

bool Model::CanIPlaceBlock(QPoint block_coords) {
  if (!map_->GetBlock(block_coords).IsAir()) {
    return false;
  }
  if (IsAnyMovingObjectInBlock(block_coords)) {
    return false;
  }
  return true;
}

bool Model::IsAnyMovingObjectInBlock(QPoint block_coords) {
  if (player_->IsInBlock(block_coords)) {
    return true;
  }
  for (auto mob : mobs_) {
    if (mob->IsInBlock(block_coords)) {
      return true;
    }
  }
  return false;
}
std::shared_ptr<const AllCraftRecipes> Model::GetAllCraftRecipes() const {
  return all_craft_recipes_;
}

bool Model::CanSpawnMobAt(QPointF pos, QPointF size) const {
  for (int j = std::floor(pos.x());
       j < std::floor(pos.x() + size.x() - constants::kEps); j++) {
    for (int i = std::floor(pos.y());
         i < std::floor(pos.y() + size.y() - constants::kEps); i++) {
      if (Model::GetInstance()->GetMap()->GetBlock(QPoint(j, i)).GetType() !=
          Block::Type::kAir) {
        return false;
      }
    }
    // mob cannot be spawned in the air
    if (Model::GetInstance()
            ->GetMap()
            ->GetBlock(
                QPoint(j, std::floor(pos.y() + size.y() + 1 - constants::kEps)))
            .GetType() == Block::Type::kAir) {
      return false;
    }
  }
  return true;
}
