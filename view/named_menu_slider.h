#ifndef VIEW_NAMED_MENU_SLIDER_H_
#define VIEW_NAMED_MENU_SLIDER_H_

#include <QScopedPointer>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>

namespace menu_named_slider_styles {

const QString kBasicStyle =
    "QLabel#name {"
    "   font-family: Comic Sans MS;"
    "   font-size: 50px;"
    "   color: black;"
    "   height: 50px;"
    "   min-width: 450px;"
    "   background-color: transparent;"
    "}"
    "QSlider {"
    "   height: 50px;"
    "}"
    "QSlider::groove:horizontal {"
    "   border: 2px solid #bbb;"
    "   background: white;"
    "   height: 40px;"
    "   border-radius: 6px;"
    "}"
    "QSlider::sub-page:horizontal {"
    "   background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #66e, "
    "stop: 1 #bbf);"
    "   background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,"
    "   stop: 0 #bbf, stop: 1 #55f);"
    "   border: 2px solid #777;"
    "   height: 40px;"
    "   border-radius: 6px;"
    "}"
    "QSlider::add-page:horizontal {"
    "   background: #fff;"
    "   border: 1px solid #777;"
    "   height: 40px;"
    "   border-radius: 6px;"
    "}"
    "QSlider::handle:horizontal {"
    "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #eee, "
    "stop:1 #ccc);"
    "   border: 2px solid #777;"
    "   width: 24px;"
    "   height: 40px;"
    "   margin-top: -2px;"
    "   margin-bottom: -2px;"
    "   border-radius: 6px;"
    "}"
    "QSlider::handle:horizontal:hover {"
    "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #fff, "
    "stop:1 #ddd);"
    "   border: 2px solid #444;"
    "   border-radius: 6px;"
    "}"
    "QSlider::sub-page:horizontal:disabled {"
    "   background: #bbb;"
    "   border-color: #999;"
    "}"
    "QSlider::add-page:horizontal:disabled {"
    "   background: #eee;"
    "   border-color: #999;"
    "}"
    "QSlider::handle:horizontal:disabled {"
    "   background: #eee;"
    "   border: 2px solid #aaa;"
    "   border-radius: 6px;"
    "}";

}  // namespace menu_named_slider_styles

class NamedMenuSlider final : public QWidget {
  Q_OBJECT

 public:
  explicit NamedMenuSlider(
      const QString& text,
      Qt::Orientation orientation = Qt::Orientation::Horizontal,
      QWidget* parent = nullptr);
  ~NamedMenuSlider() final = default;

  void setText(const QString& text) { label_->setText(text); }
  void setValue(int value) { slider_->setValue(value); }

 signals:
  void valueChanged(int value);

 public slots:
  void ChangeValue(int value) { emit(valueChanged(value)); }

 private:
  QScopedPointer<QHBoxLayout> layout_;
  QScopedPointer<QLabel> label_;
  QScopedPointer<QSlider> slider_;
};

#endif  // VIEW_NAMED_MENU_SLIDER_H_
