#ifndef VIEW_MAIN_MENU_H_
#define VIEW_MAIN_MENU_H_

#include <QHBoxLayout>
#include <QImage>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "view/abstract_menu.h"

class MainMenu final : public AbstractMenu {
  Q_OBJECT

 public:
  explicit MainMenu(QWidget* parent = nullptr);
  ~MainMenu() final = default;

  void Resize(const QSize& size) final;

  void ReTranslateButtons() final;

 private:
  void paintEvent(QPaintEvent* event) final;

  QScopedPointer<QPushButton> new_world_button_;
  QScopedPointer<QPushButton> continue_button_;
  QScopedPointer<QPushButton> settings_button_;
  QScopedPointer<QPushButton> exit_button_;

  QVBoxLayout* buttons_layout_;
  QScopedPointer<QHBoxLayout> horizontal_layout_;

  QScopedPointer<QImage> background_;
};

#endif  // VIEW_MAIN_MENU_H_
