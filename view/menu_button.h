#ifndef VIEW_MENU_BUTTON_H_
#define VIEW_MENU_BUTTON_H_

#include <QPushButton>

#include "view/abstract_menu.h"

enum class MenuButtonType {
  kBasic,
  kTransparentExpanding,
  kExpanding,
  kMenuButtonTypes
};

namespace menu_button_constants {
const QString
    kBasicStyleSheets[static_cast<int>(MenuButtonType::kMenuButtonTypes)]{
        // ---------------Basic------------------
        "QPushButton {"
        "font-family: Comic Sans MS;"
        "font-size: 60px;"
        "color: black;"
        "background-color: grey;"
        "}"
        "QPushButton:hover {"
        "font-size: 60px;"
        "color: yellow;"
        "background-color: grey;"
        "border: none;"
        "outline: none;"
        "}"
        "QPushButton:hover:pressed {"
        "border: none;"
        "color: yellow;"
        "background-color: grey;"
        "border: none;"
        "outline: none;"
        "}",
        // -------------Expanding----------------
        "QPushButton {"
        "font-family: Comic Sans MS;"
        "font-size: 70px;"
        "color: black;"
        "background: transparent;"
        "}"
        "QPushButton:hover {"
        "font-size: 90px;"
        "color: #f7e300;"
        "background: transparent;"
        "border: none;"
        "outline: none;"
        "}"
        "QPushButton:hover:pressed {"
        "border: none;"
        "color: #f7e300;"
        "background: transparent;"
        "border: none;"
        "outline: none;"
        "}",
        // ---------TransparentExpanding----------
        "QPushButton {"
        "font-family: Comic Sans MS;"
        "font-size: 70px;"
        "color: black;"
        "background-color: #7a3916;"
        "}"
        "QPushButton:hover {"
        "font-size: 90px;"
        "color: #f7e300;"
        "}"
        "QPushButton:hover:pressed {"
        "border: none;"
        "color: #f7e300;"
        "background-color: #7a3916;"
        "}"};
}

class MenuButton final : public QPushButton {
  Q_OBJECT

 public:
  explicit MenuButton(QWidget* parent = nullptr,
                      MenuButtonType button_type = MenuButtonType::kBasic);
  ~MenuButton() final = default;
};

#endif  // VIEW_MENU_BUTTON_H_
