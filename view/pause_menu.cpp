#include "view/pause_menu.h"
#include "view/menu_button.h"

#include <QPainter>

PauseMenu::PauseMenu(QWidget *parent) : AbstractMenu(parent) {
  resume_button_.reset(new MenuButton(this, MenuButtonType::kTransparentExpanding));
  auto on_resume_button_click = [this]() {
    emit(GameStateChanged(GameState::kGame));
  };
  connect(resume_button_.data(), &QPushButton::clicked, this,
          on_resume_button_click);

  settings_button_.reset(new MenuButton(this, MenuButtonType::kTransparentExpanding));
  auto on_settings_button_click = [this]() {
    emit(GameStateChanged(GameState::kSettings));
  };
  connect(settings_button_.data(), &QPushButton::clicked, this,
          on_settings_button_click);

  back_to_menu_button_.reset(new MenuButton(this, MenuButtonType::kTransparentExpanding));
  auto on_back_to_menu_button_click = [this]() {
    emit(GameStateChanged(GameState::kMainMenu));
  };
  connect(back_to_menu_button_.data(), &QPushButton::clicked, this,
          on_back_to_menu_button_click);

  horizontal_layout_.reset(new QHBoxLayout(this));
  buttons_layout_ = new QVBoxLayout;
  buttons_layout_->addStretch(1);
  buttons_layout_->addWidget(resume_button_.data());
  buttons_layout_->addWidget(settings_button_.data());
  buttons_layout_->addWidget(back_to_menu_button_.data());
  buttons_layout_->addStretch(1);

  horizontal_layout_->addStretch(2);
  horizontal_layout_->addLayout(buttons_layout_, 3);
  horizontal_layout_->addStretch(2);

  Resize(parent->size());
  ReTranslateButtons();
}

void PauseMenu::Resize(const QSize &size) {
  QWidget::resize(size);
  update();
}

void PauseMenu::ReTranslateButtons() {
  resume_button_->setText(tr("Resume"));
  settings_button_->setText(tr("Settings"));
  back_to_menu_button_->setText(tr("Back to menu"));
}

void PauseMenu::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
  QColor backgroundColor = palette().dark().color();
  backgroundColor.setAlpha(160);
  QPainter customPainter(this);
  customPainter.fillRect(rect(), backgroundColor);
  resume_button_->update();
  back_to_menu_button_->update();
}
