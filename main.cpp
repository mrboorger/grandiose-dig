#include <QApplication>
#include <QLabel>

#include "view/view.h"

int main(int argc, char* argv[]) {
  QApplication application(argc, argv);
  View* view(View::GetInstance());
  view->show();
  return QApplication::exec();
}
