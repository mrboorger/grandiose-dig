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
    "QScrollBar:vertical {"
    "   border: 0px solid #999999;"
    "   border-radius: 8px;"
    "   background: white;"
    "   width: 20px;"
    "   margin: 0px 0px 0px 0px;"
    "}"
    "QScrollBar::handle:vertical {         "
    "   min-height: 0px;"
    "   border: 0px solid red;"
    "   border-radius: 8px;"
    "   background-color: grey;"
    "}"
    "QScrollBar::add-line:vertical {"
    "   height: 0px;"
    "   subcontrol-position: bottom;"
    "   subcontrol-origin: margin;"
    "}"
    "QScrollBar::sub-line:vertical {"
    "   height: 0px;"
    "   subcontrol-position: top;"
    "   subcontrol-origin: margin;"
    "}";

}  // namespace scrollable_v_box_layout_styles

class ScrollableVBoxWidget final : public QWidget {
  Q_OBJECT

 public:
  explicit ScrollableVBoxWidget(QWidget* parent = nullptr);
  ~ScrollableVBoxWidget() final = default;

  void addWidget(QWidget* widget) { widgets_layout_->addWidget(widget); }

  void Clear();

 private:
  void resizeEvent(QResizeEvent* event) override;

  QScopedPointer<QScrollArea> scroll_area_;
  QVBoxLayout* widgets_layout_;
  QScopedPointer<QWidget> widgets_;
};

#endif  // VIEW_SCROLLABLE_V_BOX_WIDGET_H_
