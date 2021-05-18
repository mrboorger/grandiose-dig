#ifndef VIEW_SETTINGS_MENU_H_
#define VIEW_SETTINGS_MENU_H_

#include <QPushButton>
#include <QHBoxLayout>

#include "view/abstract_menu.h"
#include "view/menu_button.h"

class SettingsMenu final : public AbstractMenu {
  Q_OBJECT

 public:
  explicit SettingsMenu(QWidget* parent = nullptr);
  ~SettingsMenu() final = default;

  void Resize(const QSize& size) final;

  void ReTranslateButtons() final;

 private:
  void paintEvent(QPaintEvent* event) final;

  QScopedPointer<QPushButton> save_and_close_button_;
  QScopedPointer<QPushButton> close_button_;

  QScopedPointer<QHBoxLayout> horizontal_layout_;
  QScopedPointer<QVBoxLayout> settings_types_layout_;
};

#endif  // VIEW_SETTINGS_MENU_H_
