#ifndef VIEW_VIEW_H_
#define VIEW_VIEW_H_

#include <QHBoxLayout>
#include <QMainWindow>

#include "model/model.h"
#include "view/camera.h"

class View : public QWidget {
 public:
  static View* GetInstance();

  View(const View&) = delete;
  View(View&&) = delete;

 private:
  constexpr static int kRenderDistance = 70;

  View();

  void paintEvent(QPaintEvent* event) override;

  Camera camera_;
};

#endif  // VIEW_VIEW_H_
