#include "view/view.h"

#include <QPainter>

#include "controller/controller.h"
#include "model/constants.h"
#include "view/block_drawer.h"
#include "view/mob_drawer.h"

View* View::GetInstance() {
  static View view;
  return &view;
}

View::View()
    : QWidget(nullptr),
      camera_(QPointF(150, 150)),
      game_state_(GameState::kMainMenu),
      previous_game_state_(GameState::kMainMenu),
      drawer_(nullptr) {
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

void View::DrawGame() {
  QPainter painter(this);
  camera_.SetPoint(Model::GetInstance()->GetPlayer()->GetPosition());
  drawer_->DrawMapWithCenter(&painter, camera_.GetPoint(), rect());

  // TODO(Wind-Eagle): temporary code; need to make PlayerDrawer
  auto player = Model::GetInstance()->GetPlayer();
  QImage player_image(":/resources/textures/player.png");
  QPointF point =
      (player->GetPosition() - camera_.GetPoint()) * constants::kBlockSz +
      rect().center();
  painter.drawImage(point, player_image);
  auto mobs = Model::GetInstance()->GetMobs();
  for (auto mob : mobs) {
    QPointF mob_point =
        (mob->GetPosition() - camera_.GetPoint()) * constants::kBlockSz +
        rect().center();
    MobDrawer::DrawMob(&painter, mob_point, mob);
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
  QWidget::resizeEvent(event);
  main_menu_->Resize(event->size());
  pause_menu_->Resize(event->size());
  settings_menu_->Resize(event->size());
}

void View::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);
  switch (game_state_) {
    case GameState::kMainMenu:
      main_menu_->update();
      break;
    case GameState::kGame:
      DrawGame();
      break;
    case GameState::kPaused:
      DrawGame();
      pause_menu_->update();
      break;
    default:
      break;
  }
}
