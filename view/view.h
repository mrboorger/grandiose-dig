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
#include "view/light_map.h"
#include "view/sound_manager.h"

class View : public QOpenGLWidget {
  Q_OBJECT

 public:
  static View* GetInstance();

  View();
  View(const View&) = delete;
  View(View&&) = delete;

  ~View();

  View& operator=(const View&) = delete;
  View& operator=(View&&) = delete;

  void SetDrawer(AbstractMapDrawer* drawer) { drawer_.reset(drawer); }
  void SetLightMap(LightMap* light_map) { light_map_.reset(light_map); }
  void SetInventoryDrawer(InventoryDrawer* drawer);

  QPoint GetCursorPos() const;              // in pixels
  QPoint GetBlockCoordUnderCursor() const;  // in blocks
  QPointF GetCoordUnderCursor() const;      // in blocks

  void UpdateBlock(QPoint pos) { drawer_->UpdateBlock(pos); }
  std::shared_ptr<LightMap> GetLightMap() { return light_map_; }

 private slots:
  void DamageDealt(MovingObject* object);
  void BecameDead(MovingObject* object);
  void MobSound(MovingObject* object);

 private:
  static View* instance_;

  void initializeGL() override;
  void paintGL() override;

  void DrawPlayer(QPainter* painter);

  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

  void UpdateLight(QPoint camera_pos);

  QPointF GetTopLeftWindowCoord() const;  // in blocks

  Camera camera_;
  std::unique_ptr<SoundManager> sound_manager_;
  std::unique_ptr<AbstractMapDrawer> drawer_;
  std::shared_ptr<LightMap> light_map_;
  std::unique_ptr<InventoryDrawer> inventory_drawer_;

  bool is_visible_inventory_ = false;
};

#endif  // VIEW_VIEW_H_
