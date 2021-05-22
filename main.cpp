#include <QApplication>
#include <QDir>
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

  QDir dir;
  dir.mkdir(QDir::currentPath() + "/saves");

  Controller* controller(Controller::GetInstance());
  View* view(View::GetInstance());
  controller->CreateNewWorld("world1", 42);
  // controller->LoadFromFile("world1");

  view->show();
  view->setWindowState(Qt::WindowMaximized);
  return QApplication::exec();
}
