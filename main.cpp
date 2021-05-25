#include <QApplication>
#include <QDir>
#include <QFontDatabase>
#include <QIcon>
#include <QLibraryInfo>
#include <QSurfaceFormat>

#include "controller/controller.h"
#include "model/perlin_chunk_map_generator.h"
#include "view/view.h"

int main(int argc, char* argv[]) {
  QApplication application(argc, argv);
  QSurfaceFormat format = QSurfaceFormat::defaultFormat();
  application.setWindowIcon(QIcon(QString(":/resources/icons/icon.png")));
  format.setDepthBufferSize(24);
  QSurfaceFormat::setDefaultFormat(format);

  QCoreApplication::setOrganizationName("ШУБКА");
  QCoreApplication::setApplicationName("Grandiose dig");

  QDir dir;
  dir.mkdir(QDir::currentPath() + "/saves");
  View view;
  Controller::GetInstance();
  view.PlayMusic();
  view.show();
  view.setWindowState(Qt::WindowMaximized);
  return QApplication::exec();
}
