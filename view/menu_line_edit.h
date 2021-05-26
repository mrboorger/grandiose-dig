#ifndef VIEW_MENU_LINE_EDIT_H_
#define VIEW_MENU_LINE_EDIT_H_

#include <QLineEdit>

namespace menu_line_edit_styles {

const QString kBasicStyle =
    "QLineEdit {"
    "   font-family: Comic Sans MS;"
    "   font-size: 40px;"
    "   padding: 0 8px;"
    "   color: black;"
    "   border-radius: 12px;"
    "   background-color: #a1a39d;"
    "}"
    "QLineEdit:hover {"
    "   background-color: #838581;"
    "   outline: none;"
    "}"
    "QLineEdit:focus {"
    "   background-color: #838581;"
    "   outline: none;"
    "}";

}  // namespace menu_line_edit_styles

class MenuLineEdit final : public QLineEdit {
  Q_OBJECT

 public:
  explicit MenuLineEdit(QWidget* parent = nullptr);
  ~MenuLineEdit() final = default;
};

#endif  // VIEW_MENU_LINE_EDIT_H_
