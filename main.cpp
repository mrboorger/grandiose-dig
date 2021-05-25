#include <QApplication>
#include <QDir>
#include <QFontDatabase>
#include <QLibraryInfo>
#include <QSurfaceFormat>

#include "controller/controller.h"
#include "view/view.h"

int main(int argc, char* argv[]) {
  QApplication application(argc, argv);
  QSurfaceFormat format = QSurfaceFormat::defaultFormat();
  format.setDepthBufferSize(24);
  //  format.setStencilBufferSize(8);
  //  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);
  View view;

  QCoreApplication::setOrganizationName("ШУБКА");
  QCoreApplication::setApplicationName("Grandiose dig");

  QDir dir;
  dir.mkdir(QDir::currentPath() + "/saves");
  Controller::GetInstance();
  view.PlayMusic();
  view.show();
  view.setWindowState(Qt::WindowMaximized);
  return QApplication::exec();
}
