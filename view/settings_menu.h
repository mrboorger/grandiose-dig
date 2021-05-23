#ifndef VIEW_SETTINGS_MENU_H_
#define VIEW_SETTINGS_MENU_H_

#include <QHBoxLayout>
#include <QKeyEvent>
#include <QKeySequence>
#include <QPushButton>
#include <QScrollArea>
#include <QSettings>
#include <QSlider>
#include <QStackedWidget>
#include <QTranslator>
#include <map>

#include "controller/controller_types.h"
#include "view/abstract_menu.h"
#include "view/menu_button.h"
#include "view/named_menu_slider.h"
#include "view/scrollable_v_box_widget.h"

class SettingsMenu final : public AbstractMenu {
  Q_OBJECT

 public:
  explicit SettingsMenu(QWidget* parent = nullptr);
  ~SettingsMenu() final;

  void Resize(const QSize& size) final;

  void ReTranslateButtons() final;

  void setTransparentBackground(bool enable) {
    transparent_background_ = enable;
  }

 signals:
  void SettingsChanged();

 private:
  enum class SettingsType { kGeneral, kControls };

  void ChangeLanguage(const QString& language);

  void paintEvent(QPaintEvent* event) final;
  void keyPressEvent(QKeyEvent* event) final;

  bool transparent_background_;
  std::map<QString, QVariant> temporary_settings_changes_;

  bool listening_to_press_event_;
  QString controller_type_to_change_;
  MenuButton* button_to_update_on_press_event_;

  QString current_language_;
  QScopedPointer<QSettings> settings_;
  QScopedPointer<QTranslator> translator_;
  QScopedPointer<QTranslator> qt_translator_;

  QScopedPointer<QHBoxLayout> horizontal_layout_;
  QVBoxLayout* settings_types_layout_;

  QScopedPointer<QStackedWidget> current_settings_;
  QVBoxLayout* general_settings_layout_;
  QVBoxLayout* controls_settings_layout_;
  QVBoxLayout* language_settings_layout_;
  QScopedPointer<QWidget> general_settings_widget_;
  QScopedPointer<QWidget> controls_settings_widget_;
  QScopedPointer<QWidget> language_settings_widget_;

  QScopedPointer<MenuButton> general_settings_button_;
  QScopedPointer<MenuButton> controls_settings_button_;
  QScopedPointer<MenuButton> language_settings_button_;
  QScopedPointer<MenuButton> close_button_;
  QScopedPointer<MenuButton> save_and_close_button_;

  // General Settings
  QScopedPointer<ScrollableVBoxWidget> scrollable_generals_widget_;
  QScopedPointer<NamedMenuSlider> general_volume_slider_;
  QScopedPointer<NamedMenuSlider> music_volume_slider_;
  QScopedPointer<NamedMenuSlider> sounds_volume_slider_;

  // Controls Settings
  QScopedPointer<ScrollableVBoxWidget> scrollable_controls_widget_;
  std::vector<MenuButton*> change_key_buttons_;

  // Language Settings
  QScopedPointer<ScrollableVBoxWidget> scrollable_languages_widget_;
  QScopedPointer<MenuButton> english_language_button_;
  QScopedPointer<MenuButton> russian_language_button_;
  QScopedPointer<MenuButton> german_language_button_;
  QScopedPointer<MenuButton> ukrainian_language_button_;
};

#endif  // VIEW_SETTINGS_MENU_H_
