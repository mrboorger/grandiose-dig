#ifndef VIEW_SCROLLABLE_V_BOX_LAYOUT_H_
#define VIEW_SCROLLABLE_V_BOX_LAYOUT_H_

#include <QHBoxLayout>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

namespace scrollable_v_box_layout_styles {

const QString kBasicStyle =
    "QWidget {"
    "   background: transparent;"
    "}"
    "QScrollBar { "
    "   width: 24px;"
    "}";

}  // namespace scrollable_v_box_layout_styles

class ScrollableVBoxWidget final : public QWidget {
  Q_OBJECT

 public:
  explicit ScrollableVBoxWidget(QWidget* parent = nullptr);
  ~ScrollableVBoxWidget() final = default;

  void addWidget(QWidget* widget) { widgets_layout_->addWidget(widget); };

 private:
  QScopedPointer<QScrollArea> scroll_area_;
  QScopedPointer<QVBoxLayout> widgets_layout_;
  QScopedPointer<QWidget> widgets_;
};

#endif  // VIEW_SCROLLABLE_V_BOX_LAYOUT_H_
