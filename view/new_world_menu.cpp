#include "view/new_world_menu.h"

#include <QIntValidator>
#include <QPainter>
#include <limits>

#include "utils.h"

namespace {

const QString kLineEditLabelStyle =
    "QLabel {"
    "   font-family: Comic Sans MS;"
    "   font-size: 50px;"
    "   color: black;"
    "   background-color: transparent;"
    "}";

}  // namespace

NewWorldMenu::NewWorldMenu(QWidget *parent) : AbstractMenu(parent) {
  back_button_.reset(new MenuButton(this));
  auto on_back_button_click = [this]() {
    world_name_line_edit_->clear();
    seed_line_edit_->clear();
    emit(GameStateChanged(GameState::kMainMenu));
  };
  connect(back_button_.data(), &QPushButton::clicked, this,
          on_back_button_click);
  create_world_button_.reset(new MenuButton(this));
  auto on_create_world_button_click = [this]() {
    if (!world_name_line_edit_->text().isEmpty() &&
        !seed_line_edit_->text().isEmpty()) {
      emit(CreateNewWorldSignal(world_name_line_edit_->text(),
                                seed_line_edit_->text().toUInt()));
      world_name_line_edit_->clear();
      seed_line_edit_->clear();
      emit(GameStateChanged(GameState::kGame));
    }
  };
  connect(create_world_button_.data(), &QPushButton::clicked, this,
          on_create_world_button_click);

  settings_names_layout_.reset(new QVBoxLayout);
  world_name_label_.reset(new QLabel(this));
  world_name_label_->setStyleSheet(kLineEditLabelStyle);
  world_name_label_->setAlignment(Qt::AlignRight);
  seed_label_.reset(new QLabel(this));
  seed_label_->setStyleSheet(kLineEditLabelStyle);
  seed_label_->setAlignment(Qt::AlignRight);
  settings_names_layout_->addWidget(world_name_label_.data());
  settings_names_layout_->addWidget(seed_label_.data());

  settings_inputs_layout_.reset(new QVBoxLayout);
  world_name_line_edit_.reset(new MenuLineEdit(this));
  world_name_line_edit_->setMaxLength(30);
  seed_line_edit_.reset(new MenuLineEdit(this));
  seed_line_edit_->setValidator(new QIntValidator(
      0, std::numeric_limits<int>::max(), seed_line_edit_.data()));
  settings_inputs_layout_->addWidget(world_name_line_edit_.data());
  settings_inputs_layout_->addWidget(seed_line_edit_.data());

  settings_input_buttons_layout_.reset(new QVBoxLayout);
  random_world_name_button_.reset(new MenuButton(this, MenuButtonType::kEmpty));
  auto on_random_world_name_button_click = [this]() {
    world_name_line_edit_->setText("World" + QString::number(utils::random()));
  };
  connect(random_world_name_button_.data(), &QPushButton::clicked, this,
          on_random_world_name_button_click);
  auto world_name_pixmap = QPixmap(":/resources/images/world_name_icon.png");
  random_world_name_button_->setIcon(QIcon(world_name_pixmap));
  random_world_name_button_->setIconSize(world_name_pixmap.size());

  random_seed_button_.reset(new MenuButton(this, MenuButtonType::kEmpty));
  auto on_random_seed_button_click = [this]() {
    seed_line_edit_->setText(QString::number(utils::random()));
  };
  connect(random_seed_button_.data(), &QPushButton::clicked, this,
          on_random_seed_button_click);
  auto seed_pixmap = QPixmap(":/resources/images/seed_icon.png");
  random_seed_button_->setIcon(QIcon(seed_pixmap));
  random_seed_button_->setIconSize(seed_pixmap.size());

  settings_input_buttons_layout_->addWidget(random_world_name_button_.data());
  settings_input_buttons_layout_->addWidget(random_seed_button_.data());

  settings_layout_.reset(new QHBoxLayout);
  settings_layout_->addLayout(settings_names_layout_.data(), 2);
  settings_layout_->addLayout(settings_inputs_layout_.data(), 7);
  settings_layout_->addLayout(settings_input_buttons_layout_.data(), 1);
  buttons_layout_.reset(new QHBoxLayout);
  buttons_layout_->addWidget(back_button_.data());
  buttons_layout_->addWidget(create_world_button_.data());

  horizontal_layout_.reset(new QHBoxLayout(this));
  vertical_layout_ = new QVBoxLayout;
  vertical_layout_->addStretch(2);
  vertical_layout_->addLayout(settings_layout_.data());
  vertical_layout_->addLayout(buttons_layout_.data());
  vertical_layout_->addStretch(1);
  horizontal_layout_->addStretch(2);
  horizontal_layout_->addLayout(vertical_layout_, 4);
  horizontal_layout_->addStretch(2);

  Resize(parent->size());
  ReTranslateButtons();
}

void NewWorldMenu::Resize(const QSize &size) { QWidget::resize(size); }

void NewWorldMenu::ReTranslateButtons() {
  world_name_label_->setText(tr("Name") + ": ");
  seed_label_->setText(tr("Seed") + ": ");
  seed_label_->setMinimumWidth(world_name_label_->width());
  back_button_->setText(tr("Back"));
  create_world_button_->setText(tr("Create"));
}

void NewWorldMenu::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
  QPainter painter(this);
  painter.drawImage(0, 0,
                    QImage(":/resources/images/main_menu_background.png"));
}
