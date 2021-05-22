#include "view/main_menu.h"

#include "controller/controller.h"
#include "view/menu_button.h"

MainMenu::MainMenu(QWidget* parent) : AbstractMenu(parent) {
  new_world_button_.reset(
      new MenuButton(this, MenuButtonType::kTransparentExpanding));
  auto on_new_world_button_click = []() {
    // TODO(yaroslaffb): implement CreateNewWorldMenu class and GameState
  };
  connect(new_world_button_.data(), &QPushButton::clicked, this,
          on_new_world_button_click);

  continue_button_.reset(
      new MenuButton(this, MenuButtonType::kTransparentExpanding));
  auto on_continue_button_click = [this]() {
    emit(GameStateChanged(GameState::kGame));
  };
  connect(continue_button_.data(), &QPushButton::clicked, this,
          on_continue_button_click);

  settings_button_.reset(
      new MenuButton(this, MenuButtonType::kTransparentExpanding));
  auto on_settings_button_click = [this]() {
    emit(GameStateChanged(GameState::kSettings));
  };
  connect(settings_button_.data(), &QPushButton::clicked, this,
          on_settings_button_click);

  exit_button_.reset(
      new MenuButton(this, MenuButtonType::kTransparentExpanding));
  auto on_exit_button_click = []() { View::GetInstance()->close(); };
  connect(exit_button_.data(), &QPushButton::clicked, this,
          on_exit_button_click);

  buttons_layout_ = new QVBoxLayout;
  buttons_layout_->addStretch(2);
  buttons_layout_->addWidget(new_world_button_.data());
  buttons_layout_->addWidget(continue_button_.data());
  buttons_layout_->addWidget(settings_button_.data());
  buttons_layout_->addWidget(exit_button_.data());
  buttons_layout_->addStretch(1);

  horizontal_layout_.reset(new QHBoxLayout(this));
  horizontal_layout_->addStretch(2);
  horizontal_layout_->addLayout(buttons_layout_, 3);
  horizontal_layout_->addStretch(2);

  Resize(parent->size());
  ReTranslateButtons();
}

void MainMenu::Resize(const QSize& size) { QWidget::resize(size); }

void MainMenu::ReTranslateButtons() {
  new_world_button_->setText(tr("New World"));
  continue_button_->setText(tr("Continue"));
  settings_button_->setText(tr("Settings"));
  exit_button_->setText(tr("Exit"));
}

void MainMenu::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);
  QPainter painter(this);
  painter.drawImage(0, 0,
                    QImage(":/resources/images/main_menu_background.png"));
}
