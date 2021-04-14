#ifndef VIEW_VIEW_H_
#define VIEW_VIEW_H_

#include <QHBoxLayout>
#include <QMainWindow>
#include <memory>

#include "model/model.h"
#include "view/abstract_map_drawer.h"
#include "view/camera.h"
#include "view/inventory_drawer.h"

class View : public QWidget {
 public:
  static View* GetInstance();

  View(const View&) = delete;
  View(View&&) = delete;

  ~View() = default;

  View& operator=(const View&) = delete;
  View& operator=(View&&) = delete;

  void SetDrawer(AbstractMapDrawer* drawer) { drawer_.reset(drawer); }
  void SetInventoryDrawer(InventoryDrawer* drawer);

  QPointF GetTopLeftWindowCoord() const;  // in blocks
  QPoint GetCursorPos() const;            // in pixels

  void UpdateBlock(QPoint pos) { drawer_->UpdateBlock(pos); }

 private:
  constexpr static int kRenderDistance = 70;

  View();

  void paintEvent(QPaintEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

  Camera camera_;
  std::unique_ptr<AbstractMapDrawer> drawer_;
  std::unique_ptr<InventoryDrawer> inventory_drawer_;
};

#endif  // VIEW_VIEW_H_
