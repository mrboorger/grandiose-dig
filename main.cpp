#include <QApplication>

#include "controller/controller.h"
#include "model/flat_map_generator.h"
#include "view/view.h"

int main(int argc, char* argv[]) {
  QApplication application(argc, argv);
  View* view(View::GetInstance());

  // TODO(Wind-Eagle): delete. {
  Controller* controller(Controller::GetInstance());
  FlatMapGenerator generator;
  controller->SetGeneratedMap(&generator);
  controller->SetPlayer();
  // }
  view->show();
  return QApplication::exec();
}
