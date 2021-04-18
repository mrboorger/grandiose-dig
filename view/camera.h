#ifndef VIEW_CAMERA_H_
#define VIEW_CAMERA_H_

#include <QPointF>

class Camera {
 public:
  explicit Camera(QPointF point);

  const QPointF& GetPoint() const;
  void SetPoint(QPointF point);

  void Translate(QPointF delta);

 private:
  QPointF point_;  // center, in blocks
};

#endif  // VIEW_CAMERA_H_
