#ifndef VIEW_SCROLLABLE_V_BOX_WIDGET_H_
#define VIEW_SCROLLABLE_V_BOX_WIDGET_H_

#include <QHBoxLayout>
#include <QResizeEvent>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

namespace scrollable_v_box_layout_styles {

const QString kBasicStyle =
    "QWidget {"
    "   background-color: transparent;"
    "}"
    "QScrollBar:vertical { "
    "   width: 24px;"
    "   margin-left: -24px;"
    "}";

}  // namespace scrollable_v_box_layout_styles

class ScrollableVBoxWidget final : public QWidget {
  Q_OBJECT

 public:
  explicit ScrollableVBoxWidget(QWidget* parent = nullptr);
  ~ScrollableVBoxWidget() final = default;

  void addWidget(QWidget* widget) { widgets_layout_->addWidget(widget); };

 private:
  void resizeEvent(QResizeEvent* event) override;

  QScopedPointer<QScrollArea> scroll_area_;
  QScopedPointer<QVBoxLayout> widgets_layout_;
  QScopedPointer<QWidget> widgets_;
};

#endif  // VIEW_SCROLLABLE_V_BOX_WIDGET_H_
