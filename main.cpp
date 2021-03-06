#include <QApplication>
#include <QLabel>

int main(int argc, char* argv[]) {
  QApplication application(argc, argv);
  QLabel label(QLabel::tr("Hello world!"));
  label.show();
  return QApplication::exec();
}
