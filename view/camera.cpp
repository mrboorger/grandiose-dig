#include "view/camera.h"

Camera::Camera(QPointF point) : point_(point) {}

const QPointF& Camera::GetPoint() const { return point_; }

void Camera::SetPoint(QPointF point) { point_ = point; }

void Camera::Translate(QPointF delta) { point_ += delta; }
