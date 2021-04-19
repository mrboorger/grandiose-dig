#ifndef VIEW_PAUSE_MENU_H_
#define VIEW_PAUSE_MENU_H_

#include <QPushButton>

#include "view/abstract_menu.h"

class PauseMenu final : public AbstractMenu {
  Q_OBJECT

 public:
  explicit PauseMenu(QWidget* parent = nullptr);
  ~PauseMenu() final = default;

  void Resize(const QSize& size) final;

  void ReTranslateButtons() final;

 protected:
  void PlaceButtons() final;

 private:
  void paintEvent(QPaintEvent* event) final;

  QScopedPointer<QPushButton> resume_button_;
  QScopedPointer<QPushButton> back_to_menu_button_;
};

#endif  // VIEW_PAUSE_MENU_H_
