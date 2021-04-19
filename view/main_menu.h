#ifndef VIEW_MAIN_MENU_H_
#define VIEW_MAIN_MENU_H_

#include <QPushButton>
#include <QWidget>

#include "view/abstract_menu.h"

class MainMenu final : public AbstractMenu {
  Q_OBJECT

 public:
  explicit MainMenu(QWidget* parent = nullptr);
  ~MainMenu() final = default;

  void Resize(const QSize& size) final;

  void ReTranslateButtons() final;

 protected:
  void PlaceButtons() final;

 private:
  void paintEvent(QPaintEvent* event) final;

  QScopedPointer<QPushButton> single_player_button_;
  QScopedPointer<QPushButton> settings_button_;
  QScopedPointer<QPushButton> exit_button_;
};

#endif  // VIEW_MAIN_MENU_H_
