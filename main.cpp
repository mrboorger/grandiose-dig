#include <QApplication>
#include <QLabel>

#include "view/view.h"
#include "model/flat_map_generator.h"
#include "controller/controller.h"

int main(int argc, char* argv[]) {
  QApplication application(argc, argv);
  View* view(View::GetInstance());
  Controller* controller(Controller::GetInstance());
  FlatMapGenerator generator;
  controller->SetMap(&generator);
  view->show();
  return QApplication::exec();
}
