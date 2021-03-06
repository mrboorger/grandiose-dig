#ifndef VIEW_VIEW_H_
#define VIEW_VIEW_H_

#include <QMainWindow>
#include <QHBoxLayout>

class View : public QWidget {
 public:
  static View* GetInstance();
 private:
  View();
};

#endif  // VIEW_VIEW_H_
