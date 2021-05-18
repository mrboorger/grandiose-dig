#include "settings_menu.h"

#include <QPainter>
#include <QSettings>

SettingsMenu::SettingsMenu(QWidget *parent) : AbstractMenu(parent) {
  QSettings settings;
  if (settings.value("kLeft", -1) == -1) {
    settings.setValue("kLeft", Qt::Key::Key_Left);
  }
  if (settings.value("kRight", -1) == -1) {
    settings.setValue("kRight", Qt::Key::Key_Right);
  }
  if (settings.value("kJump", -1) == -1) {
    settings.setValue("kJump", Qt::Key::Key_Space);
  }

  close_button_.reset(new MenuButton(this));
  auto on_close_button_click = [this]() {
    emit(AbstractMenu::GameStateChanged(GameState::kSwitchingToPrevious));
  };
  connect(close_button_.data(), &QPushButton::clicked, this,
          on_close_button_click);

  save_and_close_button_.reset(new MenuButton(this));
  auto on_save_and_close_button_click = [this]() {
    emit(AbstractMenu::GameStateChanged(GameState::kSwitchingToPrevious));
  };
  connect(save_and_close_button_.data(), &QPushButton::clicked, this,
          on_save_and_close_button_click);

  settings_types_layout_.reset(new QVBoxLayout);
  settings_types_layout_->addStretch(2);
  settings_types_layout_->addWidget(close_button_.data());
  settings_types_layout_->addWidget(save_and_close_button_.data());
  settings_types_layout_->addStretch(2);
  horizontal_layout_.reset(new QHBoxLayout(this));
  horizontal_layout_->addStretch(1);
  horizontal_layout_->addLayout(settings_types_layout_.data(), 2);
  horizontal_layout_->addStretch(3);
  horizontal_layout_->addStretch(1);

  Resize(parent->size());
  ReTranslateButtons();
}

void SettingsMenu::Resize(const QSize &size) {
  QWidget::resize(size);
  update();
}

void SettingsMenu::ReTranslateButtons() {
  close_button_->setText(tr("Exit"));
  save_and_close_button_->setText(tr("Save and exit"));
}

void SettingsMenu::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
  QPainter painter(this);
  painter.drawImage(0, 0,
                    QImage(":/resources/images/main_menu_background.png"));

  close_button_->update();
}
