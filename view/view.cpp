#include "view.h"
View* View::GetInstance() {
  static View view;
  return &view;
}

View::View() : QWidget() {

}
