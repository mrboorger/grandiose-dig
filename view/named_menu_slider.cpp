#include "view/named_menu_slider.h"

NamedMenuSlider::NamedMenuSlider(const QString& text,
                                 Qt::Orientation orientation, QWidget* parent)
    : QWidget(parent) {
  layout_.reset(new QHBoxLayout);
  label_.reset(new QLabel(text, this));
  slider_.reset(new QSlider(orientation, this));
  label_->setObjectName("name");
  label_->setAlignment(Qt::AlignCenter);
  label_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  layout_->addWidget(label_.data());
  layout_->addWidget(slider_.data());
  setLayout(layout_.data());
  setStyleSheet(menu_named_slider_styles::kBasicStyle);

  connect(slider_.data(), &QSlider::valueChanged, this,
          &NamedMenuSlider::ChangeValue);
}
