#include "view/settings_menu.h"

#include <QApplication>
#include <QKeySequence>
#include <QPainter>
#include <QSettings>

SettingsMenu::SettingsMenu(QWidget* parent)
    : AbstractMenu(parent), transparent_background_(false) {
  settings_.reset(new QSettings);
  if (settings_->value("kLeft", -1) == -1) {
    settings_->setValue("kLeft", Qt::Key::Key_Left);
  }
  if (settings_->value("kRight", -1) == -1) {
    settings_->setValue("kRight", Qt::Key::Key_Right);
  }
  if (settings_->value("kJump", -1) == -1) {
    settings_->setValue("kJump", Qt::Key::Key_Space);
  }

  current_language_ = settings_->value("language", "none").toString();
  if (current_language_ == "none") {
    current_language_ = "en_US";
    settings_->setValue("language", "en_US");
  }
  translator_.reset(new QTranslator);
  qt_translator_.reset(new QTranslator);
  QApplication::installTranslator(translator_.data());
  QApplication::installTranslator(qt_translator_.data());
  ChangeLanguage(current_language_);

  general_settings_widget_.reset(new QWidget);
  general_settings_layout_.reset(new QVBoxLayout);
  general_settings_widget_->setLayout(general_settings_layout_.data());
  controls_settings_widget_.reset(new QWidget);
  controls_settings_layout_.reset(new QVBoxLayout);
  controls_settings_widget_->setLayout(controls_settings_layout_.data());
  language_settings_widget_.reset(new ScrollableVBoxWidget);
  language_settings_layout_.reset(new QVBoxLayout);
  language_settings_widget_->setLayout(language_settings_layout_.data());

  // --------------------------Settings types-----------------------------------
  current_settings_.reset(new QStackedWidget);
  current_settings_->addWidget(general_settings_widget_.data());
  current_settings_->addWidget(controls_settings_widget_.data());
  current_settings_->addWidget(language_settings_widget_.data());

  general_settings_button_.reset(new MenuButton(this));
  auto on_general_settings_button_click = [this] {
    current_settings_->setCurrentWidget(general_settings_widget_.data());
  };
  connect(general_settings_button_.data(), &QPushButton::clicked, this,
          on_general_settings_button_click);

  controls_settings_button_.reset(new MenuButton(this));
  auto on_controls_settings_button_click = [this] {
    current_settings_->setCurrentWidget(controls_settings_widget_.data());
  };
  connect(controls_settings_button_.data(), &QPushButton::clicked, this,
          on_controls_settings_button_click);

  language_settings_button_.reset(new MenuButton(this));
  auto on_language_settings_button_click = [this] {
    current_settings_->setCurrentWidget(language_settings_widget_.data());
  };
  connect(language_settings_button_.data(), &QPushButton::clicked, this,
          on_language_settings_button_click);

  close_button_.reset(new MenuButton(this));
  auto on_close_button_click = [this]() {
    QString saved_language = settings_->value("language").toString();
    if (current_language_ != saved_language) {
      ChangeLanguage(saved_language);
    }
    temporary_settings_changes_.clear();
    emit(AbstractMenu::GameStateChanged(GameState::kSwitchingToPrevious));
  };
  connect(close_button_.data(), &QPushButton::clicked, this,
          on_close_button_click);

  save_and_close_button_.reset(new MenuButton(this));
  auto on_save_and_close_button_click = [this]() {
    settings_->setValue("language", current_language_);
    for (const auto& [key, value] : temporary_settings_changes_) {
      settings_->setValue(key, value);
    }
    temporary_settings_changes_.clear();
    emit(SettingsMenu::SettingsChanged());
    emit(AbstractMenu::GameStateChanged(GameState::kSwitchingToPrevious));
  };
  connect(save_and_close_button_.data(), &QPushButton::clicked, this,
          on_save_and_close_button_click);

  settings_types_layout_.reset(new QVBoxLayout);
  settings_types_layout_->addStretch(2);
  settings_types_layout_->addWidget(general_settings_button_.data());
  settings_types_layout_->addWidget(controls_settings_button_.data());
  settings_types_layout_->addWidget(language_settings_button_.data());
  settings_types_layout_->addWidget(close_button_.data());
  settings_types_layout_->addWidget(save_and_close_button_.data());
  settings_types_layout_->addStretch(2);

  // --------------------------General Settings---------------------------------
  general_volume_slider_.reset(
      new NamedMenuSlider("", Qt::Orientation::Horizontal, this));
  general_volume_slider_->setValue(
      settings_->value("general_volume", 100).toInt());
  auto on_general_volume_slider_change = [this](int value) {
    temporary_settings_changes_["general_volume"] = value;
  };
  connect(general_volume_slider_.data(), &NamedMenuSlider::valueChanged, this,
          on_general_volume_slider_change);

  music_volume_slider_.reset(
      new NamedMenuSlider("", Qt::Orientation::Horizontal, this));
  music_volume_slider_->setValue(settings_->value("music_volume", 100).toInt());
  auto on_music_volume_slider_change = [this](int value) {
    temporary_settings_changes_["music_volume"] = value;
  };
  connect(music_volume_slider_.data(), &NamedMenuSlider::valueChanged, this,
          on_music_volume_slider_change);

  sounds_volume_slider_.reset(
      new NamedMenuSlider("", Qt::Orientation::Horizontal, this));
  sounds_volume_slider_->setValue(
      settings_->value("sounds_volume", 100).toInt());
  auto on_sounds_volume_slider_change = [this](int value) {
    temporary_settings_changes_["sounds_volume"] = value;
  };
  connect(sounds_volume_slider_.data(), &NamedMenuSlider::valueChanged, this,
          on_sounds_volume_slider_change);

  general_settings_layout_->addStretch(2);
  general_settings_layout_->addWidget(general_volume_slider_.data());
  general_settings_layout_->addWidget(music_volume_slider_.data());
  general_settings_layout_->addWidget(sounds_volume_slider_.data());
  general_settings_layout_->addStretch(2);

  // --------------------------Controls Settings--------------------------------
  controls_settings_layout_->addStretch(2);
  controls_settings_layout_->addStretch(2);

  // --------------------------Language Settings--------------------------------
  scrollable_languages_widget_.reset(new ScrollableVBoxWidget(this));

  english_language_button_.reset(
      new MenuButton(scrollable_languages_widget_.data()));
  auto on_english_language_button_click = [this]() {
    ChangeLanguage(current_language_ = "en");
  };
  connect(english_language_button_.data(), &QPushButton::clicked, this,
          on_english_language_button_click);

  russian_language_button_.reset(
      new MenuButton(scrollable_languages_widget_.data()));
  auto on_russian_language_button_click = [this]() {
    ChangeLanguage(current_language_ = "ru");
  };
  connect(russian_language_button_.data(), &QPushButton::clicked, this,
          on_russian_language_button_click);

  german_language_button_.reset(
      new MenuButton(scrollable_languages_widget_.data()));
  auto on_german_language_button_click = [this]() {
    ChangeLanguage(current_language_ = "de");
  };
  connect(german_language_button_.data(), &QPushButton::clicked, this,
          on_german_language_button_click);

  ukrainian_language_button_.reset(
      new MenuButton(scrollable_languages_widget_.data()));
  auto on_ukrainian_language_button_click = [this]() {
    ChangeLanguage(current_language_ = "uk");
  };
  connect(ukrainian_language_button_.data(), &QPushButton::clicked, this,
          on_ukrainian_language_button_click);

  scrollable_languages_widget_->addWidget(english_language_button_.data());
  scrollable_languages_widget_->addWidget(russian_language_button_.data());
  scrollable_languages_widget_->addWidget(german_language_button_.data());
  scrollable_languages_widget_->addWidget(ukrainian_language_button_.data());

  language_settings_layout_->addStretch(2);
  language_settings_layout_->addWidget(scrollable_languages_widget_.data(), 4);
  language_settings_layout_->addStretch(2);

  // ---------------------------------------------------------------------------

  horizontal_layout_.reset(new QHBoxLayout(this));
  horizontal_layout_->addStretch(1);
  horizontal_layout_->addLayout(settings_types_layout_.data(), 2);
  horizontal_layout_->addWidget(current_settings_.data(), 3);
  horizontal_layout_->addStretch(1);

  Resize(parent->size());
  ReTranslateButtons();
}

void SettingsMenu::Resize(const QSize& size) {
  QWidget::resize(size);
  update();
}

void SettingsMenu::ReTranslateButtons() {
  general_settings_button_->setText(tr("General"));
  controls_settings_button_->setText(tr("Controls"));
  language_settings_button_->setText(tr("Language"));
  close_button_->setText(tr("Exit"));
  save_and_close_button_->setText(tr("Save and exit"));

  general_volume_slider_->setText(tr("General volume"));
  music_volume_slider_->setText(tr("Music volume"));
  sounds_volume_slider_->setText(tr("Sounds volume"));

  english_language_button_->setText(tr("English"));
  russian_language_button_->setText(tr("Русский"));
  german_language_button_->setText(tr("Deutsche"));
  ukrainian_language_button_->setText(tr("Українська"));
}

void SettingsMenu::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);
  QPainter painter(this);
  if (transparent_background_) {
    QColor background_color("#a1a39d");
    background_color.setAlpha(160);
    painter.fillRect(rect(), background_color);
  } else {
    painter.drawImage(0, 0,
                      QImage(":/resources/images/main_menu_background.png"));
  }
}

void SettingsMenu::ChangeLanguage(const QString& language) {
  translator_->load(":resources/translations/translation_" + language);
  qt_translator_->load("qtbase_" + language);
}
