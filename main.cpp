#include <QApplication>
#include <QDir>
#include <QFontDatabase>
#include <QLibraryInfo>
#include <QSettings>
#include <QTranslator>

#include "controller/controller.h"
#include "model/perlin_chunk_map_manager.h"
#include "view/view.h"

int main(int argc, char* argv[]) {
  QApplication application(argc, argv);

  QCoreApplication::setOrganizationName("ШУБКА");
  QCoreApplication::setApplicationName("Grandiose dig");

  QDir dir;
  dir.mkdir(QDir::currentPath() + "/saves");

  View view;
  Controller* controller(Controller::GetInstance());
  view.show();
  view.setWindowState(Qt::WindowMaximized);
  return QApplication::exec();
}
