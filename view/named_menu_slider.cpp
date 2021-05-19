#include "named_menu_slider.h"

NamedMenuSlider::NamedMenuSlider(const QString& text,
                                 Qt::Orientation orientation, QWidget* parent)
    : QWidget(parent),
      layout_(QHBoxLayout()),
      label_(text, this),
      slider_(orientation, this) {
  setStyleSheet(menu_named_slider_styles::kBasicStyle);
  layout_.addWidget(&label_);
  layout_.addWidget(&slider_);
  setLayout(&layout_);

  connect(&slider_, &QSlider::valueChanged, this,
          &NamedMenuSlider::ChangeValue);
}
