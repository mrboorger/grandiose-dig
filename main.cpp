#include <QApplication>
#include <QFontDatabase>
#include <QLibraryInfo>
#include <QSettings>
#include <QTranslator>

#include "controller/controller.h"
#include "model/perlin_chunk_map_generator.h"
#include "view/view.h"

int main(int argc, char* argv[]) {
  QApplication application(argc, argv);

  QCoreApplication::setOrganizationName("ШУБКА");
  QCoreApplication::setApplicationName("Grandiose dig");

  View* view(View::GetInstance());
  // TODO(Wind-Eagle): delete. {
  Controller* controller(Controller::GetInstance());
  PerlinChunkMapGenerator generator(42);
  controller->SetGeneratedMap(&generator);
  controller->SetPlayer();
  controller->SetMob();
  // }
  // QApplication::installTranslator(&new_translator);
  view->show();
  view->setWindowState(Qt::WindowMaximized);
  return QApplication::exec();
}
