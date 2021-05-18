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

View* View::GetInstance() {
  static View view;
  return &view;
}

View::View()
    : QOpenGLWidget(nullptr),
      camera_(QPointF(150, 150)),
      sound_manager_(new SoundManager()),
      drawer_(nullptr),
      game_state_(GameState::kMainMenu),
      previous_game_state_(GameState::kMainMenu) {
  connect(Model::GetInstance(), &Model::DamageDealt, this, &View::DamageDealt);
  connect(Model::GetInstance(), &Model::BecameDead, this, &View::BecameDead);
  connect(Model::GetInstance(), &Model::MobSound, this, &View::MobSound);

  main_menu_.reset(new MainMenu(this));
  connect(main_menu_.data(), &AbstractMenu::GameStateChanged, this,
          &View::ChangeGameState);
  main_menu_->setVisible(true);

  pause_menu_.reset(new PauseMenu(this));
  connect(pause_menu_.data(), &AbstractMenu::GameStateChanged, this,
          &View::ChangeGameState);
  pause_menu_->setVisible(false);

  settings_menu_.reset(new SettingsMenu(this));
  connect(settings_menu_.data(), &AbstractMenu::GameStateChanged, this,
          &View::ChangeGameState);
  settings_menu_->setVisible(false);
}

void View::ChangeGameState(GameState new_state) {
  switch (game_state_) {
    case GameState::kMainMenu:
      main_menu_->setVisible(false);
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

  if (drawer_) {
    drawer_->Init();
  }
}

void View::paintGL() {
  auto* gl = GLFunctions::GetInstance();
  QPainter painter(this);
  painter.beginNativePainting();

  gl->glClear(GL_COLOR_BUFFER_BIT);

  camera_.SetPoint(Model::GetInstance()->GetPlayer()->GetPosition());
  QPointF camera_pos = camera_.GetPoint();

  light_map_->CalculateRegion(
      drawer_->GetDrawRegion(QPoint(camera_pos.x(), camera_pos.y())));
  for (auto* to_update = light_map_->UpdateList(); !to_update->empty();) {
    for (auto pos : *to_update) {
      drawer_->UpdateBlock(pos);
    }
    to_update->clear();
  }

  drawer_->DrawMapWithCenter(&painter, camera_pos, rect());

  inventory_drawer_->DrawInventory(&painter);

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
    pause_menu_->ReTranslateButtons();
    settings_menu_->ReTranslateButtons();
  }
  QWidget::changeEvent(event);
}

void View::keyPressEvent(QKeyEvent* event) {
  Controller::GetInstance()->KeyPress(event->key());
}

void View::keyReleaseEvent(QKeyEvent* event) {
  Controller::GetInstance()->KeyRelease(event->key());
}

void View::resizeEvent(QResizeEvent* event) {
  QOpenGLWidget::resizeEvent(event);
  main_menu_->Resize(event->size());
  pause_menu_->Resize(event->size());
  settings_menu_->Resize(event->size());
}

void View::paintEvent(QPaintEvent* event) {
  QOpenGLWidget::paintEvent(event);
  switch (game_state_) {
    case GameState::kMainMenu:
      main_menu_->update();
      break;
    case GameState::kGame:
      paintGL();
      break;
    case GameState::kPaused:
      paintGL();
      pause_menu_->update();
      break;
    case GameState::kSettings:
      QPainter painter(this);
      if (previous_game_state_ == GameState::kGame ||
          previous_game_state_ == GameState::kPaused) {
        paintGL();
        QColor backgroundColor("#a1a39d");
        backgroundColor.setAlpha(160);
        // painter.fillRect(rect(), backgroundColor);
      } else {
        painter.drawImage(
            0, 0, QImage(":/resources/images/main_menu_background.png"));
      }
      settings_menu_->update();
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
