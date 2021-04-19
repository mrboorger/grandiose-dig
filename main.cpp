#include <QApplication>
#include <QLibraryInfo>
#include <QSettings>
#include <QTranslator>

#include "controller/controller.h"
#include "model/flat_chunk_map_generator.h"
#include "view/view.h"

int main(int argc, char* argv[]) {
  QApplication application(argc, argv);

  QCoreApplication::setOrganizationName("ШУБКА");
  QCoreApplication::setApplicationName("Grandiose dig");

  QSettings settings;
  settings.setValue("language", "en_US");
  QString language = settings.value("language", "en_US").toString();
  QTranslator translator;
  if (translator.load(":resources/translations/" + language)) {
    QApplication::installTranslator(&translator);
  }

  QTranslator qt_base_translator;
  if (qt_base_translator.load(
          "qtbase_" + language,
          QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
    QApplication::installTranslator(&qt_base_translator);
  }

  View* view(View::GetInstance());
  // TODO(Wind-Eagle): delete. {
  Controller* controller(Controller::GetInstance());
  FlatChunkMapGenerator generator(42);
  controller->SetGeneratedMap(&generator);
  controller->SetPlayer();
  controller->SetMob();
  // }
  view->show();
  return QApplication::exec();
}
