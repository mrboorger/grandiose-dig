#include "view/select_world_menu.h"

#include <QDir>
#include <QPainter>

SelectWorldMenu::SelectWorldMenu(QWidget *parent) : AbstractMenu(parent) {
  worlds_.reset(new ScrollableVBoxWidget(this));

  horizontal_layout_.reset(new QHBoxLayout(this));
  vertical_layout_ = new QVBoxLayout;
  vertical_layout_->addStretch(2);
  vertical_layout_->addWidget(worlds_.data(), 4);
  vertical_layout_->addStretch(1);

  horizontal_layout_->addStretch(2);
  horizontal_layout_->addLayout(vertical_layout_, 4);
  horizontal_layout_->addStretch(2);

  UpdateAvailableSaves();
  Resize(parent->size());
  ReTranslateButtons();
}

void SelectWorldMenu::Resize(const QSize &size) { QWidget::resize(size); }

void SelectWorldMenu::ReTranslateButtons() {}

void SelectWorldMenu::UpdateAvailableSaves() {
  worlds_->Clear();
  QDir save_directory(QDir::currentPath() + "/saves");
  QStringList saves = save_directory.entryList(
      QStringList() << "*",
      QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
  for (const auto &save_name : saves) {
    auto *button = new MenuButton(save_name, this);
    auto on_button_click = [this, save_name]() {
      emit(LoadWorldSignal(save_name));
      emit(GameStateChanged(GameState::kGame));
    };
    connect(button, &QPushButton::clicked, this, on_button_click);
    worlds_->addWidget(button);
  }
}

void SelectWorldMenu::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
  QPainter painter(this);
  painter.drawImage(0, 0,
                    QImage(":/resources/images/main_menu_background.png"));
}
