#ifndef VIEW_PAUSE_MENU_H_
#define VIEW_PAUSE_MENU_H_

#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

#include "view/abstract_menu.h"

class PauseMenu final : public AbstractMenu {
  Q_OBJECT

 public:
  explicit PauseMenu(QWidget* parent = nullptr);
  ~PauseMenu() final = default;

  void Resize(const QSize& size) final;

  void ReTranslateButtons() final;

 private:
  void paintEvent(QPaintEvent* event) final;

  QScopedPointer<QPushButton> resume_button_;
  QScopedPointer<QPushButton> settings_button_;
  QScopedPointer<QPushButton> back_to_menu_button_;

  QVBoxLayout* buttons_layout_;
  QScopedPointer<QHBoxLayout> horizontal_layout_;
};

#endif  // VIEW_PAUSE_MENU_H_
