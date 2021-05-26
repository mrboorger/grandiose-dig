#include <QApplication>
#include <QDir>
#include <QFontDatabase>
#include <QIcon>
#include <QLibraryInfo>
#include <QSurfaceFormat>

#include "controller/controller.h"
#include "view/view.h"

int main(int argc, char* argv[]) {
  QApplication application(argc, argv);
  QSurfaceFormat format = QSurfaceFormat::defaultFormat();
  application.setWindowIcon(QIcon(QString(":/resources/icons/icon.png")));
  format.setDepthBufferSize(24);
  QSurfaceFormat::setDefaultFormat(format);

  QCoreApplication::setOrganizationName("ShUBKa");
  QCoreApplication::setApplicationName("Grandiose Dig");

  QDir dir;
  dir.mkdir(QDir::currentPath() + "/saves");
  View view;
  Controller::GetInstance();
  view.show();
  view.setWindowState(Qt::WindowMaximized);
  return QApplication::exec();
}
