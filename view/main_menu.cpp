#include "main_menu.h"

#include "controller/controller.h"

MainMenu::MainMenu(QWidget* parent) : AbstractMenu(parent) {
  single_player_button_.reset(new QPushButton(tr("Single Player"), this));
  auto on_single_player_button_click = [this]() {
    emit(GameStateChanged(GameState::kGame));
  };
  connect(single_player_button_.data(), &QPushButton::clicked, this,
          on_single_player_button_click);

  settings_button_.reset(new QPushButton(tr("Settings"), this));
  auto on_settings_button_click = [this]() {
    emit(GameStateChanged(GameState::kSettings));
  };
  connect(settings_button_.data(), &QPushButton::clicked, this,
          on_settings_button_click);

  exit_button_.reset(new QPushButton(tr("Exit"), this));
  auto on_exit_button_click = []() { View::Quit(); };
  connect(exit_button_.data(), &QPushButton::clicked, this,
          on_exit_button_click);

  Resize(parent->size());
}

void MainMenu::Resize(QSize size) {
  resize(size);
  PlaceButtons();
}

void MainMenu::setVisible(bool visible) {
  for (const auto& child :
      findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly)) {
    child->setVisible(visible);
  }
}

void MainMenu::PlaceButtons() {
  single_player_button_->setGeometry(width() / 2 - 80, height() / 2 - 45 - 90,
                                     160, 90);
  settings_button_->setGeometry(width() / 2 - 80, height() / 2 - 45 + 10, 160,
                                90);
  exit_button_->setGeometry(width() / 2 - 80, height() / 2 - 45 + 110, 160, 90);
}
