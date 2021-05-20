#include "view/scrollable_v_box_layout.h"

ScrollableVBoxWidget::ScrollableVBoxWidget(QWidget *parent) : QWidget(parent) {
  scroll_area_.reset(new QScrollArea(this));
  widgets_.reset(new QWidget);
  scroll_area_->setWidget(widgets_.data());
  widgets_layout_.reset(new QVBoxLayout(widgets_.data()));

  scroll_area_->setAlignment(Qt::AlignRight);
  scroll_area_->setBackgroundRole(QPalette::Dark);
  scroll_area_->setWidgetResizable(true);
  scroll_area_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  scroll_area_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  setStyleSheet(scrollable_v_box_layout_styles::kBasicStyle);
}
