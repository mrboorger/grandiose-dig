#include <QApplication>
#include <QIcon>
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
  View view;

  // TODO(Wind-Eagle): delete. {
  Controller* controller(Controller::GetInstance());
  PerlinChunkMapGenerator generator(42);
  controller->SetGeneratedMap(&generator);
  controller->SetPlayer();
  controller->SetMob();
  view.PlayMusic();
  // }
  view.show();
  view.setWindowState(Qt::WindowMaximized);
  return QApplication::exec();
}
