#ifndef VIEW_VIEW_H_
#define VIEW_VIEW_H_

#include <QHBoxLayout>
#include <QMainWindow>

#include "model/model.h"
#include "view/camera.h"

class View : public QWidget {
  Q_OBJECT
 public:
  static View* GetInstance();

  View(const View&) = delete;
  View(View&&) = delete;

  void paintEvent(QPaintEvent* event) override;

 private:
  constexpr static int kRenderDistance = 70;

  View();

  Camera camera_;
};

#endif  // VIEW_VIEW_H_
