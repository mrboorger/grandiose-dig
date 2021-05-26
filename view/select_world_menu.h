#ifndef VIEW_SELECT_WORLD_MENU_H_
#define VIEW_SELECT_WORLD_MENU_H_

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QScopedPointer>
#include <QVBoxLayout>

#include "view/abstract_menu.h"
#include "view/menu_button.h"
#include "view/scrollable_v_box_widget.h"

class SelectWorldMenu final : public AbstractMenu {
  Q_OBJECT

 public:
  explicit SelectWorldMenu(QWidget* parent = nullptr);
  ~SelectWorldMenu() final = default;

  void Resize(const QSize& size) final;

  void ReTranslateButtons() final;

  void UpdateAvailableSaves();

 signals:
  void LoadWorldSignal(const QString& world_name);

 private:
  void paintEvent(QPaintEvent* event) final;

  QScopedPointer<QHBoxLayout> horizontal_layout_;
  QVBoxLayout* vertical_layout_;

  QScopedPointer<ScrollableVBoxWidget> worlds_;
};

#endif  // VIEW_SELECT_WORLD_MENU_H_
