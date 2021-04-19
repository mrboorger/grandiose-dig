#include "view/main_menu.h"

#include "controller/controller.h"

MainMenu::MainMenu(QWidget* parent) : AbstractMenu(parent) {
  single_player_button_.reset(new QPushButton(this));
  auto on_single_player_button_click = [this]() {
    emit(GameStateChanged(GameState::kGame));
  };
  connect(single_player_button_.data(), &QPushButton::clicked, this,
          on_single_player_button_click);

  settings_button_.reset(new QPushButton(this));
  auto on_settings_button_click = [this]() {
    emit(GameStateChanged(GameState::kSettings));
  };
  connect(settings_button_.data(), &QPushButton::clicked, this,
          on_settings_button_click);

  exit_button_.reset(new QPushButton(this));
  auto on_exit_button_click = []() { View::Quit(); };
  connect(exit_button_.data(), &QPushButton::clicked, this,
          on_exit_button_click);

  Resize(parent->size());
  PlaceButtons();
  ReTranslateButtons();
}

void MainMenu::Resize(const QSize& size) {
  QWidget::resize(size);
  PlaceButtons();
}

void MainMenu::PlaceButtons() {
  single_player_button_->setGeometry(width() / 2 - 80, height() / 2 - 45 - 90,
                                     160, 90);
  settings_button_->setGeometry(width() / 2 - 80, height() / 2 - 45 + 10, 160,
                                90);
  exit_button_->setGeometry(width() / 2 - 80, height() / 2 - 45 + 110, 160, 90);
}

void MainMenu::ReTranslateButtons() {
  single_player_button_->setText(tr("Single Player"));
  settings_button_->setText(tr("Settings"));
  exit_button_->setText(tr("Exit"));
}

void MainMenu::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);
  single_player_button_->update();
  settings_button_->update();
  exit_button_->update();
}
