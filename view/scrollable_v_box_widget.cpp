#include "view/scrollable_v_box_widget.h"

ScrollableVBoxWidget::ScrollableVBoxWidget(QWidget *parent) : QWidget(parent) {
  setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

  scroll_area_.reset(new QScrollArea(this));
  widgets_.reset(new QWidget(scroll_area_.data()));
  scroll_area_->setWidget(widgets_.data());
  widgets_layout_ = new QVBoxLayout(widgets_.data());

  widgets_->setObjectName("widgets");
  scroll_area_->setAlignment(Qt::AlignRight);
  scroll_area_->setBackgroundRole(QPalette::Dark);
  scroll_area_->setWidgetResizable(true);
  scroll_area_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  scroll_area_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  setStyleSheet(scrollable_v_box_layout_styles::kBasicStyle);
}

void ScrollableVBoxWidget::resizeEvent(QResizeEvent *event) {
  QWidget::resizeEvent(event);
  scroll_area_->resize(event->size());
}

void ScrollableVBoxWidget::Clear() {
  QLayoutItem* item;
  while ((item = widgets_layout_->takeAt(0))) {
    delete item->widget();
  }
}
