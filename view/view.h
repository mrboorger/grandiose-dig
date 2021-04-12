#ifndef VIEW_VIEW_H_
#define VIEW_VIEW_H_

#include <QHBoxLayout>
#include <QMainWindow>
#include <QScopedPointer>
#include <memory>

#include "model/model.h"
#include "view/abstract_map_drawer.h"
#include "view/camera.h"
#include "view/main_menu.h"

class View : public QWidget {
 Q_OBJECT

 public:
  static View* GetInstance();

  View(const View&) = delete;
  View(View&&) = delete;

  ~View() override = default;

  View& operator=(const View&) = delete;
  View& operator=(View&&) = delete;

  static GameState GetGameState() { return GetInstance()->game_state_; };

  void SetDrawer(AbstractMapDrawer* drawer) { drawer_.reset(drawer); }

  static void Quit() { GetInstance()->close(); }

 public slots:
  void ChangeGameState(GameState new_state);

 private:
  constexpr static int kRenderDistance = 70;

  View();

  void DrawGame();

  void paintEvent(QPaintEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;

  Camera camera_;
  GameState game_state_;
  std::unique_ptr<AbstractMapDrawer> drawer_;
  QScopedPointer<MainMenu> main_menu_;
};

#endif  // VIEW_VIEW_H_
