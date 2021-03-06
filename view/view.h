#ifndef VIEW_VIEW_H_
#define VIEW_VIEW_H_

#include <QMainWindow>
#include <QHBoxLayout>

#include "camera.h"
#include "model/model.h"

class View : public QWidget {
 public:
  static View* GetInstance();

 private:
  View();

  void paintEvent(QPaintEvent* event) override;

  Camera camera_;
};

#endif  // VIEW_VIEW_H_
