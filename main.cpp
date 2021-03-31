#include <QApplication>

#include "controller/controller.h"
#include "model/flat_chunk_map_generator.h"
#include "view/view.h"

int main(int argc, char* argv[]) {
  QApplication application(argc, argv);
  View* view(View::GetInstance());

  // TODO(Wind-Eagle): delete. {
  Controller* controller(Controller::GetInstance());
  FlatChunkMapGenerator generator;
  controller->SetGeneratedMap(&generator);
  controller->SetPlayer();
  controller->SetMob();
  // }
  view->show();
  return QApplication::exec();
}
