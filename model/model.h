#ifndef MODEL_MODEL_H_
#define MODEL_MODEL_H_

#include <QDir>
#include <QFile>
#include <memory>
#include <set>
#include <unordered_set>
#include <utility>

#include "model/map.h"
#include "model/mob.h"
#include "model/player.h"

class Model : public QObject {
  Q_OBJECT

 public:
  static Model* GetInstance();

  Model(const Model&) = delete;
  Model(Model&&) = delete;

  ~Model() = default;

  Model& operator=(const Model&) = delete;
  Model& operator=(Model&&) = delete;

  std::shared_ptr<AbstractMap> GetMap() { return map_; }

  void SetMap(std::shared_ptr<AbstractMap> map) { map_ = std::move(map); }

  std::shared_ptr<Player> GetPlayer() const { return player_; }

  void SetPlayer(const std::shared_ptr<Player>& player) { player_ = player; }

  void AddMob(const std::shared_ptr<Mob>& mob) { mobs_.insert(mob); }
  void DeleteMob(const std::shared_ptr<Mob>& mob) { mobs_.erase(mob); }
  const std::set<std::shared_ptr<Mob>>& GetMobs() const { return mobs_; }

  void MoveObjects(const std::unordered_set<ControllerTypes::Key>& pressed_keys,
                   double time);

  void PickItemToPlayer(InventoryItem item) { player_->PickItem(item); }

  bool CanSpawnMobAt(QPointF pos, QPointF size) const;

  void Read(const QJsonObject& json);
  void Write(QJsonObject& json) const;

  void SetSaveFileName(const QString& save_file_name) {
    current_save_file_name_ = save_file_name;
  }

  bool LoadFromFile(const QString& file_name);
  bool SaveToFile(const QString& file_name);
  bool SaveToFile() { return SaveToFile(current_save_file_name_); }

  void Clear();

 signals:
  void DamageDealt(MovingObject* object);
  void BecameDead(MovingObject* object);
  void MobSound(MovingObject* object);

 private:
  Model() = default;

  QString current_save_file_name_;
  std::set<std::shared_ptr<Mob>> mobs_;
  std::shared_ptr<AbstractMap> map_;
  std::shared_ptr<Player> player_;
};

#endif  // MODEL_MODEL_H_
