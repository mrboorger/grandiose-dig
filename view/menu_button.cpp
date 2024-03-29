#include "view/menu_button.h"
#include <qpushbutton.h>

MenuButton::MenuButton(QWidget* parent, MenuButtonType button_type)
    : QPushButton(parent) {
  if (button_type == MenuButtonType::kTransparentExpanding) {
    setFlat(true);
  }
  setStyleSheet(
      menu_button_styles::kBasicStyles[static_cast<int>(button_type)]);
  setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
}

MenuButton::MenuButton(const QString& text, QWidget* parent,
                       MenuButtonType button_type) : QPushButton(parent) {
  setText(text);
  if (button_type == MenuButtonType::kTransparentExpanding) {
    setFlat(true);
  }
  setStyleSheet(
      menu_button_styles::kBasicStyles[static_cast<int>(button_type)]);
  setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
}
