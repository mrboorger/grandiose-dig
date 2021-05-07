#ifndef VIEW_VIEW_H_
#define VIEW_VIEW_H_

#include <QHBoxLayout>
#include <QOpenGLWidget>
#include <memory>

#include "model/model.h"
#include "view/abstract_map_drawer.h"
#include "view/camera.h"
#include "view/gl_map_drawer.h"
#include "view/inventory_drawer.h"
#include "view/sound_manager.h"

class View : public QOpenGLWidget {
  Q_OBJECT

 public:
  static View* GetInstance();

  View(const View&) = delete;
  View(View&&) = delete;

  ~View() = default;

  View& operator=(const View&) = delete;
  View& operator=(View&&) = delete;

  void SetDrawer(AbstractMapDrawer* drawer) { drawer_.reset(drawer); }
  void SetInventoryDrawer(InventoryDrawer* drawer);

  QPoint GetCursorPos() const;              // in pixels
  QPoint GetBlockCoordUnderCursor() const;  // in blocks
  QPointF GetCoordUnderCursor() const;      // in blocks

  void UpdateBlock(QPoint pos) { drawer_->UpdateBlock(pos); }

 private slots:
  void DamageDealt(MovingObject::Type type);
  void BecameDead(MovingObject::Type type);
  void MobSound(MovingObject::Type type);

 private:
  constexpr static int kRenderDistance = 70;
  View();

  void initializeGL() override;
  void paintGL() override;

  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

  QPointF GetTopLeftWindowCoord() const;  // in blocks

  Camera camera_;
  std::unique_ptr<SoundManager> sound_manager_;
  std::unique_ptr<AbstractMapDrawer> drawer_;
  std::unique_ptr<InventoryDrawer> inventory_drawer_;
};

#endif  // VIEW_VIEW_H_
