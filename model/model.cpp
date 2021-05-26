#include "model/model.h"

#include <QCborMap>
#include <QCborValue>
#include <QJsonArray>
#include <QJsonDocument>
#include <ctime>
#include <random>
#include <vector>

#include "utils.h"

Model* Model::GetInstance() {
  static Model model;
  return &model;
}

int Model::CheckPlayerPosition() {
  for (int y = -200; y <= 200; y++) {
    if (Model::GetInstance()->GetMap()->GetBlock(QPoint(0, y)).IsAir() &&
        Model::GetInstance()->GetMap()->GetBlock(QPoint(1, y)).IsAir() &&
        (!Model::GetInstance()->GetMap()->GetBlock(QPoint(0, y + 1)).IsAir() ||
         !Model::GetInstance()->GetMap()->GetBlock(QPoint(1, y + 1)).IsAir())) {
      return y;
    }
  }
  return -200;
}

void Model::MoveObjects(
    const std::unordered_set<ControllerTypes::Key>& pressed_keys, double time) {
  // TODO(Wind-Eagle): this is temporary code.
  if (player_->IsDead()) {
    player_->DeleteAllEffects();
    player_->SetHealth(constants::kPlayerHealth);
    player_->SetPosition(QPointF(0.0, CheckPlayerPosition()));
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

bool Model::CanPlaceBlock(QPoint block_coords) {
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
std::shared_ptr<const CraftRecipeCollection> Model::GetCraftRecipeCollection()
    const {
  return all_craft_recipes_;
}

bool Model::CanSpawnMobAt(QPointF pos, QPointF size) const {
  for (int j = std::floor(pos.x()); j < std::floor(pos.x() + size.x()); j++) {
    for (int i = std::floor(pos.y() - constants::kEps);
         i < std::floor(pos.y() + size.y() - constants::kEps); i++) {
      if (Model::GetInstance()
              ->GetMap()
              ->GetBlock(QPoint(j, i))
              .GetFrontType() != Block::FrontType::kAir) {
        return false;
      }
    }
    // mob cannot be spawned in the air
    if (Model::GetInstance()
            ->GetMap()
            ->GetBlock(
                QPoint(j, std::floor(pos.y() + size.y() + 1 - constants::kEps)))
            .GetFrontType() == Block::FrontType::kAir) {
      return false;
    }
  }
  return true;
}

void Model::Read(const QJsonObject& json) {
  player_ = std::make_shared<Player>(QPointF(0, 0));
  player_->Read(json["player"].toObject());
  QJsonArray mobs = json["mobs"].toArray();
  mobs_.clear();
  for (auto mob_json : mobs) {
    auto* mob = new Mob(QPointF(0, 0), Mob::Type::kZombie);
    mob->Read(mob_json.toObject());
    mobs_.emplace(mob);
  }
  current_world_seed_ = json["seed"].toString().toUInt();
}

void Model::Write(QJsonObject* json) const {
  QJsonObject player;
  player_->Write(&player);
  (*json)["player"] = player;
  QJsonArray mobs;
  for (const auto& mob : mobs_) {
    QJsonObject mob_object;
    mob->Write(&mob_object);
    mobs.append(mob_object);
  }
  (*json)["mobs"] = mobs;
  (*json)["seed"] = QString::number(current_world_seed_);
}

bool Model::LoadFromFile(const QString& file_name) {
  Clear();
  QFile save_file(file_name + "/entities");
  if (!save_file.open(QIODevice::ReadOnly)) {
    qWarning("Couldn't open load file.");
    return false;
  }

  current_save_file_name_ = file_name;
  QByteArray save_data = save_file.readAll();
  QJsonDocument world(
      QJsonDocument(QCborValue::fromCbor(save_data).toMap().toJsonObject()));
  Read(world.object());
  return true;
}

bool Model::SaveToFile(const QString& file_name) {
  if (file_name.isEmpty()) {
    return false;
  }
  QFile save_file(file_name + "/entities");
  if (!save_file.open(QIODevice::WriteOnly)) {
    qWarning("Couldn't open save file.");
    return false;
  }

  current_save_file_name_ = file_name;
  QJsonObject world;
  Write(&world);
  save_file.write(QCborValue::fromJsonValue(world).toCbor());
  return true;
}

void Model::Clear() {
  if (!current_save_file_name_.isEmpty()) {
    SaveToFile();
  }
}

void Model::DespawnMobs() {
  for (auto i = mobs_.begin(), last = mobs_.end(); i != last;) {
    if (utils::GetDistance(Model::GetInstance()->GetPlayer()->GetPosition(),
                           (*i)->GetPosition()) >
        constants::kMobsDespawnRadius) {
      i = mobs_.erase(i);
    } else {
      i++;
    }
  }
}
