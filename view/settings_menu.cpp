#include "settings_menu.h"

SettingsMenu::SettingsMenu(QWidget *parent) : AbstractMenu(parent) {
  back_button_.reset(new QPushButton(this));
  auto on_back_button_click = [this]() {
    emit(AbstractMenu::GameStateChanged(GameState::kSwitchingToPrevious));
  };
  connect(back_button_.data(), &QPushButton::clicked, this,
          on_back_button_click);

  Resize(parent->size());
  PlaceButtons();
  ReTranslateButtons();
}

void SettingsMenu::Resize(const QSize &size) {
  QWidget::resize(size);
  PlaceButtons();
  update();
}

void SettingsMenu::ReTranslateButtons() { back_button_->setText(tr("Back")); }

void SettingsMenu::PlaceButtons() {
  back_button_->setGeometry(0, height() / 2, 160, 90);
}

void SettingsMenu::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
  back_button_->update();
}
