#include "view/view.h"

#include <QColor>
#include <QPainter>
#include <chrono>
#include <cmath>
#include <ctime>
#include <random>

#include "controller/controller.h"
#include "model/constants.h"
#include "utils.h"
#include "view/block_drawer.h"
#include "view/gl_func.h"
#include "view/moving_object_drawer.h"

View* View::instance_ = nullptr;

View* View::GetInstance() { return instance_; }

View::View()
    : QOpenGLWidget(nullptr),
      camera_(QPointF(150, 150)),
      sound_manager_(new SoundManager()),
      drawer_(nullptr),
      should_initialize_drawer_(false),
      game_state_(GameState::kMainMenu),
      previous_game_state_(GameState::kMainMenu) {
  assert(!instance_);
  instance_ = this;
  connect(Model::GetInstance(), &Model::DamageDealt, this, &View::DamageDealt);
  connect(Model::GetInstance(), &Model::BecameDead, this, &View::BecameDead);
  connect(Model::GetInstance(), &Model::MobSound, this, &View::MobSound);

  main_menu_.reset(new MainMenu(this));
  connect(main_menu_.data(), &AbstractMenu::GameStateChanged, this,
          &View::ChangeGameState);
  main_menu_->setVisible(true);

  new_world_menu_.reset(new NewWorldMenu(this));
  connect(new_world_menu_.data(), &AbstractMenu::GameStateChanged, this,
          &View::ChangeGameState);
  connect(new_world_menu_.data(), &NewWorldMenu::CreateNewWorldsSignal, this,
          &View::CreateNewWorld);
  new_world_menu_->setVisible(false);

  pause_menu_.reset(new PauseMenu(this));
  connect(pause_menu_.data(), &AbstractMenu::GameStateChanged, this,
          &View::ChangeGameState);
  pause_menu_->setVisible(false);

  settings_menu_.reset(new SettingsMenu(this));
  connect(settings_menu_.data(), &AbstractMenu::GameStateChanged, this,
          &View::ChangeGameState);
  connect(settings_menu_.data(), &SettingsMenu::SettingsChanged, this,
          &View::UpdateSettings);
  settings_menu_->setVisible(false);
}

View::~View() { instance_ = nullptr; }

void View::ChangeGameState(GameState new_state) {
  switch (game_state_) {
    case GameState::kMainMenu:
      main_menu_->setVisible(false);
      break;
    case GameState::kNewWorldMenu:
      new_world_menu_->setVisible(false);
      break;
    case GameState::kPaused:
      pause_menu_->setVisible(false);
      break;
    case GameState::kSettings:
      settings_menu_->setVisible(false);
      break;
    default:
      break;
  }
  switch (new_state) {
    case GameState::kMainMenu:
      main_menu_->setVisible(true);
      main_menu_->setFocus();
      break;
    case GameState::kNewWorldMenu:
      new_world_menu_->setVisible(true);
      new_world_menu_->setFocus();
      break;
    case GameState::kGame:
      setFocus();
      break;
    case GameState::kPaused:
      pause_menu_->setVisible(true);
      pause_menu_->setFocus();
      break;
    case GameState::kSettings:
      settings_menu_->setVisible(true);
      settings_menu_->setFocus();
      break;
    case GameState::kSwitchingToPrevious:
      ChangeGameState(previous_game_state_);
      return;
    default:
      break;
  }
  previous_game_state_ = game_state_;
  game_state_ = new_state;
  repaint();
}

void View::UpdateSettings() { sound_manager_->UpdateVolumes(); }

void View::SetInventoryDrawer(InventoryDrawer* drawer) {
  inventory_drawer_.reset(drawer);
}

void View::DrawPlayer(QPainter* painter) {
  auto player = Model::GetInstance()->GetPlayer();
  QPointF point =
      (player->GetPosition() - camera_.GetPoint()) * constants::kBlockSz +
      rect().center();

  MovingObjectDrawer::DrawPlayer(painter, point);
  // TODO(Wind-Eagle): make animation for block breaking: will be done when
  // blocks will not break immediately
}

void View::initializeGL() {
  assert(context());
  makeCurrent();
  auto* gl = GLFunctions::GetInstance();
  gl->initializeOpenGLFunctions();
  gl->glClearColor(1.0F, 1.0F, 1.0F, 1.0F);

  if (should_initialize_drawer_) {
    drawer_->Init();
    should_initialize_drawer_ = false;
  }
}

void View::paintGL() {
  auto* gl = GLFunctions::GetInstance();
  if (should_initialize_drawer_) {
    drawer_->Init();
    should_initialize_drawer_ = false;
  }
  QPainter painter(this);
  painter.beginNativePainting();

  gl->glClear(GL_COLOR_BUFFER_BIT);

  camera_.SetPoint(Model::GetInstance()->GetPlayer()->GetPosition());
  QPointF camera_pos = camera_.GetPoint();

  UpdateLight(QPoint(camera_pos.x(), camera_pos.y()));
  drawer_->DrawMapWithCenter(&painter, camera_pos, rect());

  if (is_visible_inventory_) {
    inventory_drawer_->DrawInventory(&painter);
  }

  // TODO(Wind-Eagle): temporary code; need to make PlayerDrawer
  DrawPlayer(&painter);
  auto mobs = Model::GetInstance()->GetMobs();
  for (auto mob : mobs) {
    QPointF mob_point =
        (mob->GetPosition() - camera_.GetPoint()) * constants::kBlockSz +
        rect().center();
    MovingObjectDrawer::DrawMob(&painter, mob_point, mob);
  }
  painter.endNativePainting();
}

void View::DamageDealt(MovingObject* object) {
  static std::uniform_int_distribution<int> distrib(0, 1);
  switch (object->GetType()) {
    case MovingObject::Type::kPlayer: {
      sound_manager_->PlaySound(
          SoundManager::SoundIndex(SoundManager::Sound::kPlayerDamage));
      break;
    }
    case MovingObject::Type::kMob: {
      int id = static_cast<Mob*>(object)->GetId();
      if (distrib(utils::random) == 0) {
        sound_manager_->PlaySound(SoundManager::SoundIndex(
            SoundManager::Sound::kMob, id, SoundManager::MobSound::kDamage1));
      } else {
        sound_manager_->PlaySound(SoundManager::SoundIndex(
            SoundManager::Sound::kMob, id, SoundManager::MobSound::kDamage2));
      }
      break;
    }
    default: {
      assert(false);
    }
  }
}

void View::BecameDead(MovingObject* object) {
  int id = static_cast<Mob*>(object)->GetId();
  sound_manager_->PlaySound(SoundManager::SoundIndex(
      SoundManager::Sound::kMob, id, SoundManager::MobSound::kDeath));
}

void View::MobSound(MovingObject* object) {
  int id = static_cast<Mob*>(object)->GetId();
  static std::uniform_int_distribution<int> distrib(0, 1);
  if (distrib(utils::random) == 0) {
    sound_manager_->PlaySound(SoundManager::SoundIndex(
        SoundManager::Sound::kMob, id, SoundManager::MobSound::kIdle1));
  } else {
    sound_manager_->PlaySound(SoundManager::SoundIndex(
        SoundManager::Sound::kMob, id, SoundManager::MobSound::kIdle2));
  }
}

void View::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    main_menu_->ReTranslateButtons();
    new_world_menu_->ReTranslateButtons();
    pause_menu_->ReTranslateButtons();
    settings_menu_->ReTranslateButtons();
  } else {
    QWidget::changeEvent(event);
  }
}

void View::keyPressEvent(QKeyEvent* event) {
  if (event->key() == Qt::Key_Escape) {
    is_visible_inventory_ = !is_visible_inventory_;
    inventory_drawer_->SetCraftMenuVisible(is_visible_inventory_);
  }
  Controller::GetInstance()->KeyPress(event->key());
}

void View::keyReleaseEvent(QKeyEvent* event) {
  Controller::GetInstance()->KeyRelease(event->key());
}

void View::resizeEvent(QResizeEvent* event) {
  QOpenGLWidget::resizeEvent(event);
  main_menu_->Resize(event->size());
  new_world_menu_->Resize(event->size());
  pause_menu_->Resize(event->size());
  settings_menu_->Resize(event->size());
}

void View::paintEvent(QPaintEvent* event) {
  switch (game_state_) {
    case GameState::kMainMenu:
      main_menu_->update();
      break;
    case GameState::kNewWorldMenu:
      new_world_menu_->update();
      break;
    case GameState::kGame:
      paintGL();
      break;
    case GameState::kPaused:
      paintGL();
      pause_menu_->update();
      break;
    case GameState::kSettings:
      if (previous_game_state_ == GameState::kGame ||
          previous_game_state_ == GameState::kPaused) {
        paintGL();
        settings_menu_->setTransparentBackground(true);
      } else {
        settings_menu_->setTransparentBackground(false);
      }
      settings_menu_->update();
      break;
    default:
      break;
  }
}

void View::mousePressEvent(QMouseEvent* event) {
  Controller::GetInstance()->ButtonPress(event->button());
}

void View::mouseReleaseEvent(QMouseEvent* event) {
  Controller::GetInstance()->ButtonRelease(event->button());
}

QPoint View::GetCursorPos() const {
  return QCursor::pos() - geometry().topLeft();
}

QPointF View::GetCoordUnderCursor() const {
  return GetTopLeftWindowCoord() +
         QPointF(GetCursorPos()) / constants::kBlockSz;
}

QPoint View::GetBlockCoordUnderCursor() const {
  QPointF pos = GetCoordUnderCursor();
  return QPoint(std::floor(pos.x()), std::floor(pos.y()));
}

QPointF View::GetTopLeftWindowCoord() const {
  return camera_.GetPoint() - QPointF(rect().center()) / constants::kBlockSz;
}

void View::CreateNewWorld(const QString& name, uint32_t seed) {
  emit(CreateNewWorldSignal(name, seed));
}

void View::UpdateLight(QPoint camera_pos) {
  light_map_->CalculateRegion(drawer_->GetDrawRegion(camera_pos));
  const auto& to_update = light_map_->TakeUpdateList();
  for (auto pos : to_update) {
    drawer_->UpdateBlock(pos);
  }
  light_map_->ClearUpdateList();
}
