#include "view/named_menu_slider.h"

NamedMenuSlider::NamedMenuSlider(const QString& text,
                                 Qt::Orientation orientation, QWidget* parent)
    : QWidget(parent),
      layout_(QHBoxLayout()),
      label_(text, this),
      slider_(orientation, this) {
  label_.setAlignment(Qt::AlignCenter);
  label_.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  layout_.addWidget(&label_);
  layout_.addWidget(&slider_);
  setLayout(&layout_);
  setStyleSheet(menu_named_slider_styles::kBasicStyle);

  connect(&slider_, &QSlider::valueChanged, this,
          &NamedMenuSlider::ChangeValue);
}