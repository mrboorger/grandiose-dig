#ifndef VIEW_SETTINGS_MENU_H_
#define VIEW_SETTINGS_MENU_H_

#include <QPushButton>

#include "abstract_menu.h"

class SettingsMenu final : public AbstractMenu {
  Q_OBJECT

 public:
  explicit SettingsMenu(QWidget* parent = nullptr);
  ~SettingsMenu() final = default;

  void Resize(const QSize& size) final;

  void ReTranslateButtons() final;

 protected:
  void PlaceButtons() final;

 private:
  void paintEvent(QPaintEvent* event) final;

  QScopedPointer<QPushButton> back_button_;
};

#endif  // VIEW_SETTINGS_MENU_H_
