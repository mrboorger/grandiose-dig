#include "controller/controller.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <memory>
#include <utility>

#include "model/abstract_map_generator.h"
#include "model/chunk_map.h"
#include "model/constants.h"
#include "model/model.h"
#include "model/perlin_chunk_map_generator.h"
#include "view/abstract_map_drawer.h"
#include "view/buffered_map_drawer.h"
#include "view/gl_map_drawer.h"
#include "view/map_drawer.h"
#include "view/view.h"

Controller* Controller::GetInstance() {
  static Controller controller;
  return &controller;
}

Controller::Controller() : tick_timer_(), save_timer_() {
  View* view = View::GetInstance();
  Model* model = Model::GetInstance();
  connect(view, &View::CreateNewWorldSignal, this, &Controller::CreateNewWorld);
  connect(view, &View::LoadWorldSignal, this, &Controller::LoadFromFile);

  tick_timer_.callOnTimeout([this]() { TickEvent(); });
  tick_timer_.start(constants::kTickDurationMsec);
  save_timer_.callOnTimeout([this]() { SaveEvent(); });
  save_timer_.start(
      settings_.value("save_duration", constants::kSaveDurationMsec).toInt());
}

void Controller::SetGeneratedMap(AbstractMapGenerator* generator) {
  auto map = std::shared_ptr<AbstractMap>(generator->GenerateMap());
  Model::GetInstance()->SetMap(map);
  View::GetInstance()->SetLightMap(new LightMap(map));
  View::GetInstance()->SetDrawer(
      new GLMapDrawer(map, View::GetInstance()->GetLightMap()));
}

void Controller::SetPlayer() {
  // TODO(Wind-Eagle): this is temporary code.
  Model::GetInstance()->SetPlayer(
      std::make_shared<Player>(QPointF(125.0, 115.0)));
  View::GetInstance()->SetInventoryDrawer(
      new InventoryDrawer(Model::GetInstance()->GetPlayer()->GetInventory()));
}

void Controller::SetMob() {
  // TODO(Wind-Eagle): this is temporary code.
  Model::GetInstance()->AddMob(
      std::make_shared<Mob>(QPointF(162.0, 104.0), Mob::Type::kQuiox));
}

void Controller::BreakBlock(double time) {
  QPoint block_coords = View::GetInstance()->GetBlockCoordUnderCursor();
  if (Model::GetInstance()->GetPlayer()->IsBlockReachableForTool(
          block_coords)) {
    Model::GetInstance()->GetPlayer()->SetDirection(
        (View::GetInstance()->GetBlockCoordUnderCursor().x() <
         Model::GetInstance()->GetPlayer()->GetPosition().x())
            ? utils::Direction::kLeft
            : utils::Direction::kRight);
    if (Model::GetInstance()->GetMap()->HitBlock(block_coords, 1.0 * time)) {
      View::GetInstance()->UpdateBlock(block_coords);
      View::GetInstance()->GetLightMap()->UpdateLight(block_coords);
    }
  }
}

void Controller::UseItem() {
  if (!Model::GetInstance()->GetPlayer()->CanUseItem()) {
    return;
  }
  QPoint block_coords = View::GetInstance()->GetBlockCoordUnderCursor();
  const InventoryItem& item =
      Model::GetInstance()->GetPlayer()->GetInventory()->GetSelectedItem();
  if (item.IsBlock()) {
    if (Model::GetInstance()->GetPlayer()->IsBlockReachableForTool(
            block_coords) &&
        Model::GetInstance()->CanIPlaceBlock(block_coords)) {
      Model::GetInstance()->GetMap()->SetBlock(
          block_coords, InventoryItem::GetBlockFromItem(item));
      View::GetInstance()->UpdateBlock(block_coords);
      Model::GetInstance()->GetPlayer()->UseItem();
    }
  } else {
    // UsePotion?
  }
  Model::GetInstance()->GetPlayer()->SetUseItemCooldownInterval();
}

void Controller::StartAttack() {
  if (!Model::GetInstance()->GetPlayer()->CanStartAttack()) {
    return;
  }
  QPointF click_coord = View::GetInstance()->GetCoordUnderCursor();
  Model::GetInstance()->GetPlayer()->SetAttackDirection(
      (click_coord.x() < Model::GetInstance()->GetPlayer()->GetPosition().x())
          ? utils::Direction::kLeft
          : utils::Direction::kRight);
  Model::GetInstance()->GetPlayer()->SetDirection(
      Model::GetInstance()->GetPlayer()->GetAttackDirection());
  Model::GetInstance()->GetPlayer()->SetAttackTick(
      constants::kPlayerAttackTime);
  Model::GetInstance()->GetPlayer()->SetAttackCooldownInterval(
      constants::kPlayerAttackCooldown);
}

bool Controller::IsVisible(QPointF player_center, QPointF mob_point) const {
  double x1 = player_center.x();
  double y1 = player_center.y();
  double x2 = mob_point.x();
  double y2 = mob_point.y();
  if (y1 < y2) {
    std::swap(x1, x2);
    std::swap(y1, y2);
  }
  QPointF old_center(x1, y1);
  QPointF new_center(x2, y2);
  int end_y = std::floor(new_center.y());
  int begin_y = std::floor(old_center.y());
  int dist = std::max(std::abs(end_y - begin_y), 1);
  for (int y = begin_y; y >= end_y; y--) {
    double center_left_x =
        utils::DivideSegment(old_center, new_center,
                             static_cast<double>(std::abs(end_y - y)) / dist)
            .x();
    double center_right_x =
        utils::DivideSegment(
            old_center, new_center,
            static_cast<double>(std::abs(std::min(dist, end_y - y + 1))) / dist)
            .x();
    for (double x = center_left_x + constants::kEps;; x += 1) {
      QPoint block_pos{static_cast<int>(std::floor(x)),
                       static_cast<int>(std::floor(y))};
      if (Model::GetInstance()->GetMap()->GetBlock(block_pos).GetType() !=
          Block::Type::kAir) {
        return false;
      }
      if (x >= std::floor(center_right_x)) {
        break;
      }
    }
  }
  return true;
}

bool Controller::CanAttackMobAtPoint(QPointF mob_point, QPointF player_center,
                                     double lower_angle,
                                     double upper_angle) const {
  if (Model::GetInstance()->GetPlayer()->IsAttackDirectionLeft()) {
    mob_point *= -1;
  }
  double distance = std::hypot(mob_point.x(), mob_point.y());
  double angle = std::atan2(mob_point.y(), mob_point.x());
  if (distance < constants::kEps) {
    return true;
  }
  return angle >= lower_angle && angle <= upper_angle &&
         distance <= constants::kPlayerAttackRadius &&
         IsVisible(player_center, mob_point + player_center);
}

bool Controller::CanAttackMob(std::shared_ptr<MovingObject> mob,
                              QPointF player_center, double lower_angle,
                              double upper_angle) const {
  auto check = [&mob, &player_center, &lower_angle, &upper_angle,
                this](QPointF pos_on_mob) {
    QPointF pos_on_mob_scaled(pos_on_mob.x() * mob->GetSize().x(),
                              pos_on_mob.y() * mob->GetSize().y());
    return CanAttackMobAtPoint(
        mob->GetPosition() + pos_on_mob_scaled - player_center, player_center,
        lower_angle, upper_angle);
  };

  return check(QPointF(0.5, 0.5)) || check(QPointF(0.0, 0.0)) ||
         check(QPointF(0.0, 1.0)) || check(QPointF(1.0, 0.0)) ||
         check(QPointF(1.0, 1.0));
}

void Controller::PlayerAttack(double time) {
  Model::GetInstance()->GetPlayer()->DecAttackCooldownInterval(time);
  if (Model::GetInstance()->GetPlayer()->IsAttackFinished()) {
    return;
  }
  Model::GetInstance()->GetPlayer()->DecAttackTick(time);
  double attack_tick = Model::GetInstance()->GetPlayer()->GetAttackTick();
  double lower_angle = constants::kPlayerLowerAttackAngle +
                       constants::kPlayerAngleTick * attack_tick;
  double upper_angle = constants::kPlayerLowerAttackAngle +
                       (constants::kPlayerAngleTick) * (attack_tick + time);
  QPointF player_center = Model::GetInstance()->GetPlayer()->GetPosition() +
                          Model::GetInstance()->GetPlayer()->GetSize() / 2;
  for (auto mob : Model::GetInstance()->GetMobs()) {
    if (CanAttackMob(mob, player_center, lower_angle, upper_angle)) {
      Damage damage(Model::GetInstance()->GetPlayer()->GetPosition(),
                    Damage::Type::kPlayer,
                    Model::GetInstance()->GetPlayer()->GetDamage(),
                    constants::kPlayerDamageAcceleration);
      mob->DealDamage(damage);
    }
  }
}

void Controller::TickEvent() {
  if (pressed_keys_.count(ControllerTypes::Key::kExit)) {
    pressed_keys_.erase(ControllerTypes::Key::kExit);
    switch (View::GetInstance()->GetGameState()) {
      case GameState::kGame:
        View::GetInstance()->ChangeGameState(GameState::kPaused);
        break;
      case GameState::kNewWorldMenu:
        View::GetInstance()->ChangeGameState(GameState::kMainMenu);
        break;
      case GameState::kSelectWorldMenu:
        View::GetInstance()->ChangeGameState(GameState::kMainMenu);
        break;
      case GameState::kMainMenu:
        View::GetInstance()->close();
        break;
      case GameState::kPaused:
        View::GetInstance()->ChangeGameState(GameState::kMainMenu);
        break;
      case GameState::kSettings:
        View::GetInstance()->ChangeGameState(GameState::kSwitchingToPrevious);
      default:
        break;
    }
  } else if (View::GetInstance()->GetGameState() == GameState::kGame) {
    if (pressed_keys_.count(ControllerTypes::Key::kShowInventory)) {
      View::GetInstance()->SwitchInventory();
      pressed_keys_.erase(ControllerTypes::Key::kShowInventory);
    }
    for (const auto& key : pressed_keys_) {
      ParseInventoryKey(key);
    }
    auto cur = std::chrono::high_resolution_clock::now();
    double time =
        std::chrono::duration_cast<std::chrono::milliseconds>(cur - prev_time_)
            .count();
    prev_time_ = cur;
    Model::GetInstance()->GetPlayer()->DecItemUsingCooldownInterval(time);
    Model::GetInstance()->MoveObjects(pressed_keys_, time);

    if (is_pressed_left_mouse_button) {
      BreakBlock(time);
      StartAttack();
    }
    if (is_pressed_right_mouse_button) {
      UseItem();
    }
    PlayerAttack(time);
    View::GetInstance()->repaint();
  }
}

void Controller::SaveEvent() {
  if (View::GetInstance()->GetGameState() == GameState::kGame) {
    SaveToFile();
  }
}

ControllerTypes::Key Controller::TranslateKeyCode(int key_code) {
  for (int key = 0; key < ControllerTypes::kKeysCount; ++key) {
    if (key_code ==
        GetInstance()
            ->settings_.value("controller_key" + QString::number(key))
            .toInt()) {
      return static_cast<ControllerTypes::Key>(key);
    }
  }
  return ControllerTypes::Key::kUnused;
}

void Controller::KeyPress(int key) {
  pressed_keys_.insert(TranslateKeyCode(key));
}

void Controller::KeyRelease(int key) {
  pressed_keys_.erase(TranslateKeyCode(key));
}

void Controller::PickItemToPlayer(InventoryItem item) {
  Model::GetInstance()->PickItemToPlayer(item);
}

void Controller::ButtonPress(Qt::MouseButton button) {
  if (button == Qt::MouseButton::LeftButton) {
    is_pressed_left_mouse_button = true;
  } else if (button == Qt::MouseButton::RightButton) {
    is_pressed_right_mouse_button = true;
  }
}

void Controller::ButtonRelease(Qt::MouseButton button) {
  if (button == Qt::MouseButton::LeftButton) {
    is_pressed_left_mouse_button = false;
  } else if (button == Qt::MouseButton::RightButton) {
    is_pressed_right_mouse_button = false;
  }
}

void Controller::CreateNewWorld(const QString& world_name, uint32_t seed) {
  Model::GetInstance()->Clear();
  Model::GetInstance()->SetSaveFileName(world_name);
  // Temporary code
  PerlinChunkMapGenerator generator(seed);
  SetGeneratedMap(&generator);
  SetPlayer();
  SetMob();
}

void Controller::LoadFromFile(const QString& file_name) {
  Model::GetInstance()->LoadFromFile(file_name);
  View::GetInstance()->SetInventoryDrawer(
      new InventoryDrawer(Model::GetInstance()->GetPlayer()->GetInventory()));

  // TODO(yaroslaffb): implement map saving instead of this temporary code {
  PerlinChunkMapGenerator generator(42);
  auto map = std::shared_ptr<AbstractMap>(generator.GenerateMap());
  Model::GetInstance()->SetMap(map);
  View::GetInstance()->SetLightMap(new LightMap(map));
  View::GetInstance()->SetDrawer(
      new GLMapDrawer(map, View::GetInstance()->GetLightMap()));
  // }
}

void Controller::ParseInventoryKey(ControllerTypes::Key translated_key) {
  if (translated_key >= ControllerTypes::Key::kInventory0 &&
      translated_key <= ControllerTypes::Key::kInventory9) {
    Model::GetInstance()->GetPlayer()->GetInventory()->ChangeSelectedItem(
        static_cast<int>(translated_key) -
        static_cast<int>(ControllerTypes::Key::kInventory0));
  }
}

void Controller::TryCraft(const CraftRecipe& recipe) {
  Model::GetInstance()->GetPlayer()->TryCraft(recipe);
}
