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
#include "view/settings_menu.h"
#include "view/pause_menu.h"

class View : public QWidget {
 public:
  static View* GetInstance();

  View(const View&) = delete;
  View(View&&) = delete;

  ~View() override = default;

  View& operator=(const View&) = delete;
  View& operator=(View&&) = delete;

  GameState GetGameState() { return game_state_; };

  void SetDrawer(AbstractMapDrawer* drawer) { drawer_.reset(drawer); }

  static void Quit() { GetInstance()->close(); }

 public slots:
  void ChangeGameState(GameState new_state);

 private:
  constexpr static int kRenderDistance = 70;

  View();

  void DrawGame();

  void changeEvent(QEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
  void paintEvent(QPaintEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;

  Camera camera_;
  std::unique_ptr<AbstractMapDrawer> drawer_;
  QScopedPointer<MainMenu> main_menu_;
  QScopedPointer<PauseMenu> pause_menu_;
  QScopedPointer<SettingsMenu> settings_menu_;
  GameState game_state_;
  GameState previous_game_state_;
};

#endif  // VIEW_VIEW_H_
