#ifndef CONTROLLER_CONTROLLER_H_
#define CONTROLLER_CONTROLLER_H_

#include <QJsonObject>
#include <QKeyEvent>
#include <QSettings>
#include <QTimer>
#include <memory>
#include <unordered_set>

#include "controller/controller_types.h"
#include "model/abstract_map_generator.h"
#include "model/model.h"
#include "model/player.h"
#include "view/view.h"

class Controller : QObject {
  Q_OBJECT

 public:
  static Controller* GetInstance();

  Controller(const Controller&) = delete;
  Controller(Controller&&) = delete;

  ~Controller() = default;

  Controller& operator=(const Controller&) = delete;
  Controller& operator=(Controller&&) = delete;

  void SetGeneratedMap(AbstractMapGenerator* generator);

  void SetPlayer();
  void SetMob();

  static ControllerTypes::Key TranslateKeyCode(int key_code);

  void KeyPress(int key);
  void KeyRelease(int key);
  void ButtonPress(Qt::MouseButton button);
  void ButtonRelease(Qt::MouseButton button);

  void PickItemToPlayer(InventoryItem item);

 public slots:
  void CreateNewWorld(const QString& world_name, int generator_seed);
  void LoadFromFile(const QString& file_name);
  void SaveToFile(const QString& file_name) {
    Model::GetInstance()->SaveToFile(file_name);
  }
  void SaveToFile() { Model::GetInstance()->SaveToFile(); }

 private:
  Controller();

  void TickEvent();
  void SaveEvent();

  void BreakBlock();
  void StartAttack();
  void PlayerAttack(double time);

  bool IsVisible(QPointF player_center, QPointF mob_point) const;
  bool CanAttackMobAtPoint(QPointF mob_point, QPointF player_center,
                           double lower_angle, double upper_angle) const;
  bool CanAttackMob(std::shared_ptr<MovingObject> mob, QPointF player_center,
                    double lower_angle, double upper_angle) const;

  QTimer tick_timer_;
  QTimer save_timer_;
  QSettings settings_;
  std::unordered_set<ControllerTypes::Key> pressed_keys_;
  bool is_pressed_right_mouse_button = false;

  std::chrono::time_point<std::chrono::high_resolution_clock> prev_time_ =
      std::chrono::high_resolution_clock::now();
};

#endif  // CONTROLLER_CONTROLLER_H_
