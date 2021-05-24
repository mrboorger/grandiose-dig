#include "view/settings_menu.h"

#include <QApplication>
#include <QKeySequence>
#include <QPainter>
#include <QSettings>

#include "controller/controller_types.h"

SettingsMenu::SettingsMenu(QWidget* parent)
    : AbstractMenu(parent),
      transparent_background_(false),
      listening_to_press_event_(false) {
  settings_.reset(new QSettings);
  if (!settings_->contains("controller_key0")) {
    settings_->setValue("controller_key0", Qt::Key::Key_Left);
  }
  if (!settings_->contains("controller_key1")) {
    settings_->setValue("controller_key1", Qt::Key::Key_Right);
  }
  if (!settings_->contains("controller_key2")) {
    settings_->setValue("controller_key2", Qt::Key::Key_Space);
  }
  if (!settings_->contains("controller_key3")) {
    settings_->setValue("controller_key3", Qt::Key::Key_I);
  }
  for (int index = 0; index < 10; ++index) {
    if (!settings_->contains("controller_key" + QString::number(4 + index))) {
      settings_->setValue("controller_key" + QString::number(4 + index),
                          Qt::Key::Key_1 + (index == 9 ? -1 : index));
    }
  }
  if (!settings_->contains("controller_key14")) {
    settings_->setValue("controller_key14", Qt::Key::Key_Escape);
  }

  current_language_ = settings_->value("language", "en").toString();
  translator_.reset(new QTranslator);
  qt_translator_.reset(new QTranslator);
  QApplication::installTranslator(translator_.data());
  QApplication::installTranslator(qt_translator_.data());

  general_settings_widget_.reset(new QWidget);
  general_settings_layout_ = new QVBoxLayout;
  general_settings_widget_->setLayout(general_settings_layout_);
  controls_settings_widget_.reset(new QWidget);
  controls_settings_layout_ = new QVBoxLayout;
  controls_settings_widget_->setLayout(controls_settings_layout_);
  language_settings_widget_.reset(new ScrollableVBoxWidget);
  language_settings_layout_ = new QVBoxLayout;
  language_settings_widget_->setLayout(language_settings_layout_);

  current_settings_.reset(new QStackedWidget(this));
  current_settings_->addWidget(general_settings_widget_.data());
  current_settings_->addWidget(controls_settings_widget_.data());
  current_settings_->addWidget(language_settings_widget_.data());

  // --------------------------Settings types-----------------------------------
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

  settings_types_layout_ = new QVBoxLayout;
  settings_types_layout_->addStretch(2);
  settings_types_layout_->addWidget(general_settings_button_.data());
  settings_types_layout_->addWidget(controls_settings_button_.data());
  settings_types_layout_->addWidget(language_settings_button_.data());
  settings_types_layout_->addWidget(close_button_.data());
  settings_types_layout_->addWidget(save_and_close_button_.data());
  settings_types_layout_->addStretch(2);

  // --------------------------General Settings---------------------------------
  scrollable_generals_widget_.reset(new ScrollableVBoxWidget(this));
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

  scrollable_generals_widget_->addWidget(general_volume_slider_.data());
  scrollable_generals_widget_->addWidget(music_volume_slider_.data());
  scrollable_generals_widget_->addWidget(sounds_volume_slider_.data());

  general_settings_layout_->addStretch(2);
  general_settings_layout_->addWidget(scrollable_generals_widget_.data(), 4);
  general_settings_layout_->addStretch(2);

  // --------------------------Controls Settings--------------------------------
  scrollable_controls_widget_.reset(new ScrollableVBoxWidget(this));
  change_key_buttons_.reserve(ControllerTypes::kKeysCount);
  for (int index = 0; index < ControllerTypes::kKeysCount; ++index) {
    change_key_buttons_.emplace_back(new MenuButton(this));
    auto on_change_key_button_click = [this, index]() {
      setFocus();
      listening_to_press_event_ = true;
      controller_key_to_change_ = "controller_key" + QString::number(index);
    };
    connect(change_key_buttons_[index], &QPushButton::clicked, this,
            on_change_key_button_click);
    scrollable_controls_widget_->addWidget(change_key_buttons_[index]);
  }

  controls_settings_layout_->addStretch(2);
  controls_settings_layout_->addWidget(scrollable_controls_widget_.data(), 4);
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
  horizontal_layout_->addStretch(2);
  horizontal_layout_->addLayout(settings_types_layout_, 5);
  horizontal_layout_->addWidget(current_settings_.data(), 9);
  horizontal_layout_->addStretch(2);

  Resize(parent->size());
  ChangeLanguage(current_language_);
}

SettingsMenu::~SettingsMenu() {
  for (auto& button : change_key_buttons_) {
    delete button;
  }
}

void SettingsMenu::Resize(const QSize& size) { QWidget::resize(size); }

void SettingsMenu::ReTranslateButtons() {
  general_settings_button_->setText(tr("General"));
  controls_settings_button_->setText(tr("Controls"));
  language_settings_button_->setText(tr("Language"));
  close_button_->setText(tr("Exit"));
  save_and_close_button_->setText(tr("Save and exit"));

  general_volume_slider_->setText(tr("General volume"));
  music_volume_slider_->setText(tr("Music volume"));
  sounds_volume_slider_->setText(tr("Sounds volume"));

  change_key_buttons_.resize(ControllerTypes::kKeysCount);
  change_key_buttons_[0]->setText(tr("Move left"));
  change_key_buttons_[1]->setText(tr("Move right"));
  change_key_buttons_[2]->setText(tr("Jump"));
  change_key_buttons_[3]->setText(tr("Show/Hide inventory"));
  for (int index = 0; index < 10; ++index) {
    change_key_buttons_[4 + index]->setText(tr("Select slot ") +
                                            QString::number(index + 1));
  }
  change_key_buttons_.back()->setText("Exit");
  for (int index = 0; index < ControllerTypes::kKeysCount; ++index) {
    QString key_str = "controller_key" + QString::number(index);
    int key = settings_->value(key_str).toInt();
    if (temporary_settings_changes_.count(key_str)) {
      key = temporary_settings_changes_[key_str].toInt();
    }
    change_key_buttons_[index]->setText(change_key_buttons_[index]->text() +
                                        ": " + QKeySequence(key).toString());
  }

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

void SettingsMenu::keyPressEvent(QKeyEvent* event) {
  if (listening_to_press_event_) {
    temporary_settings_changes_[controller_key_to_change_] = event->key();
    listening_to_press_event_ = false;
    ReTranslateButtons();
  }
}
