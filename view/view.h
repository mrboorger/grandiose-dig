#ifndef VIEW_VIEW_H_
#define VIEW_VIEW_H_

#include <QHBoxLayout>
#include <QOpenGLWidget>
#include <QScopedPointer>
#include <memory>

#include "model/model.h"
#include "view/abstract_map_drawer.h"
#include "view/camera.h"
#include "view/gl_map_drawer.h"
#include "view/inventory_drawer.h"
#include "view/light_map.h"
#include "view/main_menu.h"
#include "view/pause_menu.h"
#include "view/settings_menu.h"
#include "view/sound_manager.h"

class View : public QOpenGLWidget {
  Q_OBJECT

 public:
  static View* GetInstance();

  View(const View&) = delete;
  View(View&&) = delete;

  ~View() override = default;

  View& operator=(const View&) = delete;
  View& operator=(View&&) = delete;

  void SetDrawer(AbstractMapDrawer* drawer) { drawer_.reset(drawer); }
  void SetLightMap(LightMap* light_map) { light_map_.reset(light_map); }
  void SetInventoryDrawer(InventoryDrawer* drawer);

  GameState GetGameState() { return game_state_; }

  QPoint GetCursorPos() const;              // in pixels
  QPoint GetBlockCoordUnderCursor() const;  // in blocks
  QPointF GetCoordUnderCursor() const;      // in blocks

  void UpdateBlock(QPoint pos) { drawer_->UpdateBlock(pos); }

  std::shared_ptr<LightMap> GetLightMap() { return light_map_; }

 public slots:
  void ChangeGameState(GameState new_state);
  void UpdateSettings();

 private slots:
  void DamageDealt(MovingObject* object);
  void BecameDead(MovingObject* object);
  void MobSound(MovingObject* object);

 private:
  View();

  void initializeGL() override;
  void paintGL() override;

  void DrawPlayer(QPainter* painter);

  void changeEvent(QEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void paintEvent(QPaintEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;

  QPointF GetTopLeftWindowCoord() const;  // in blocks

  Camera camera_;
  std::unique_ptr<SoundManager> sound_manager_;
  std::unique_ptr<AbstractMapDrawer> drawer_;
  std::unique_ptr<InventoryDrawer> inventory_drawer_;
  std::shared_ptr<LightMap> light_map_;

  QScopedPointer<MainMenu> main_menu_;
  QScopedPointer<PauseMenu> pause_menu_;
  QScopedPointer<SettingsMenu> settings_menu_;
  GameState game_state_;
  GameState previous_game_state_;
};

#endif  // VIEW_VIEW_H_
