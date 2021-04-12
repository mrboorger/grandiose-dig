#ifndef VIEW_MAIN_MENU_H_
#define VIEW_MAIN_MENU_H_

#include <QPushButton>
#include <QWidget>

#include "view/abstract_menu.h"

class MainMenu : public AbstractMenu {
  Q_OBJECT

 public:
  explicit MainMenu(QWidget* parent = nullptr);
  ~MainMenu() override = default;

  void Resize(QSize size);

  void setVisible(bool visible) override;

 protected:
  void PlaceButtons() override;

 private:
  QScopedPointer<QPushButton> single_player_button_;
  QScopedPointer<QPushButton> settings_button_;
  QScopedPointer<QPushButton> exit_button_;
};

#endif  // VIEW_MAIN_MENU_H_
