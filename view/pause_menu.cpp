#include "view/pause_menu.h"

#include <QPainter>

PauseMenu::PauseMenu(QWidget *parent) : AbstractMenu(parent) {
  resume_button_.reset(new QPushButton(this));
  auto on_resume_button_click = [this]() {
    emit(GameStateChanged(GameState::kGame));
  };
  connect(resume_button_.data(), &QPushButton::clicked, this,
          on_resume_button_click);

  back_to_menu_button_.reset(new QPushButton(this));
  auto on_back_to_menu_button_click = [this]() {
    emit(GameStateChanged(GameState::kMainMenu));
  };
  connect(back_to_menu_button_.data(), &QPushButton::clicked, this,
          on_back_to_menu_button_click);

  Resize(parent->size());
  PlaceButtons();
  ReTranslateButtons();
}

void PauseMenu::Resize(const QSize &size) {
  QWidget::resize(size);
  PlaceButtons();
  update();
}

void PauseMenu::ReTranslateButtons() {
  resume_button_->setText(tr("Resume"));
  back_to_menu_button_->setText(tr("Back to menu"));
}

void PauseMenu::PlaceButtons() {
  resume_button_->setGeometry(width() / 2 - 80, height() / 2 - 45, 160, 90);
  back_to_menu_button_->setGeometry(width() / 2 - 80, height() / 2 - 45 + 100,
                                    160, 90);
}

void PauseMenu::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
  QColor backgroundColor = palette().dark().color();
  backgroundColor.setAlpha(100);
  QPainter customPainter(this);
  customPainter.fillRect(rect(), backgroundColor);
  resume_button_->update();
  back_to_menu_button_->update();
}
