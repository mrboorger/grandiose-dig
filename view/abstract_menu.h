#ifndef VIEW_BASE_MENU_H_
#define VIEW_BASE_MENU_H_

#include <QWidget>

enum class GameState { kMainMenu, kSettings, kGame, kPaused };

class AbstractMenu : public QWidget {
  Q_OBJECT

 public:
  explicit AbstractMenu(QWidget* parent = nullptr) : QWidget(parent){};
  AbstractMenu(const AbstractMenu&) = delete;
  AbstractMenu(AbstractMenu&&) = delete;

  AbstractMenu& operator=(const AbstractMenu&) = delete;
  AbstractMenu& operator=(AbstractMenu&&) = delete;

  ~AbstractMenu() = default;

 signals:
  void GameStateChanged(GameState);

 protected:
  virtual void PlaceButtons() = 0;
};

#endif  // VIEW_BASE_MENU_H_
