#include "view/menu_line_edit.h"

MenuLineEdit::MenuLineEdit(QWidget *parent) : QLineEdit(parent) {
  setStyleSheet(menu_line_edit_styles::kBasicStyle);
}
