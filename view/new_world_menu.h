#ifndef VIEW_NEW_WORLD_MENU_H_
#define VIEW_NEW_WORLD_MENU_H_

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QScopedPointer>
#include <QVBoxLayout>

#include "view/abstract_menu.h"
#include "view/menu_button.h"
#include "view/menu_line_edit.h"

class NewWorldMenu final : public AbstractMenu {
  Q_OBJECT

 public:
  explicit NewWorldMenu(QWidget* parent = nullptr);
  ~NewWorldMenu() final = default;

  void Resize(const QSize& size) final;

  void ReTranslateButtons() final;

 signals:
  void CreateNewWorldSignal(const QString& world_name, uint32_t seed);

 private:
  void paintEvent(QPaintEvent* event) final;

  QScopedPointer<QHBoxLayout> horizontal_layout_;
  QVBoxLayout* vertical_layout_;

  QScopedPointer<QHBoxLayout> settings_layout_;
  QScopedPointer<QHBoxLayout> buttons_layout_;
  QScopedPointer<QVBoxLayout> settings_names_layout_;
  QScopedPointer<QVBoxLayout> settings_inputs_layout_;
  QScopedPointer<QVBoxLayout> settings_input_buttons_layout_;

  QScopedPointer<QLabel> world_name_label_;
  QScopedPointer<MenuLineEdit> world_name_line_edit_;
  QScopedPointer<MenuButton> random_world_name_button_;
  QScopedPointer<QLabel> seed_label_;
  QScopedPointer<MenuLineEdit> seed_line_edit_;
  QScopedPointer<MenuButton> random_seed_button_;
  QScopedPointer<MenuButton> back_button_;
  QScopedPointer<MenuButton> create_world_button_;
};

#endif  // VIEW_NEW_WORLD_MENU_H_
