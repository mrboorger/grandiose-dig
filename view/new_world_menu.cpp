#include "view/new_world_menu.h"

#include <QIntValidator>
#include <QPainter>

namespace {

const QString kLineEditLabelStyle =
    "QLabel {"
    "   font-family: Comic Sans MS;"
    "   font-size: 40px;"
    "   color: black;"
    "   background-color: transparent;"
    "}";

}  // namespace

NewWorldMenu::NewWorldMenu(QWidget *parent) : AbstractMenu(parent) {
  horizontal_layout_.reset(new QHBoxLayout);
  vertical_layout_.reset(new QVBoxLayout);
  setLayout(horizontal_layout_.data());

  create_world_button_.reset(new MenuButton(this));
  auto on_create_world_button_click = [this]() {
    if (!world_name_line_edit_->text().isEmpty()) {
      emit(CreateNewWorldsSignal(world_name_line_edit_->text(),
                                 seed_line_edit_->text().toUInt()));
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

  settings_layout_.reset(new QHBoxLayout);
  settings_layout_->addLayout(settings_names_layout_.data());
  settings_layout_->addLayout(settings_inputs_layout_.data());

  vertical_layout_->addStretch(2);
  vertical_layout_->addLayout(settings_layout_.data());
  vertical_layout_->addWidget(create_world_button_.data());
  vertical_layout_->addStretch(1);

  horizontal_layout_->addStretch(2);
  horizontal_layout_->addLayout(vertical_layout_.data(), 4);
  horizontal_layout_->addStretch(2);

  Resize(parent->size());
  ReTranslateButtons();
}

void NewWorldMenu::Resize(const QSize &size) { QWidget::resize(size); }

void NewWorldMenu::ReTranslateButtons() {
  world_name_label_->setText(tr("World name:"));
  seed_label_->setText(tr("Seed:"));
  seed_label_->setMinimumWidth(world_name_label_->width());
  create_world_button_->setText("Create world");
}

void NewWorldMenu::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
  QPainter painter(this);
  painter.drawImage(0, 0,
                    QImage(":/resources/images/main_menu_background.png"));
}
