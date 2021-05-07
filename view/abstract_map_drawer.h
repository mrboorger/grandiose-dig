#ifndef VIEW_ABSTRACT_MAP_DRAWER_H_
#define VIEW_ABSTRACT_MAP_DRAWER_H_

#include <QPainter>
#include <QPointF>

class AbstractMapDrawer {
 public:
  AbstractMapDrawer() = default;
  AbstractMapDrawer(const AbstractMapDrawer&) = default;
  AbstractMapDrawer(AbstractMapDrawer&&) = default;

  virtual ~AbstractMapDrawer() = default;

  AbstractMapDrawer& operator=(const AbstractMapDrawer&) = default;
  AbstractMapDrawer& operator=(AbstractMapDrawer&&) = default;

  virtual void Init() {}
  virtual void DrawMapWithCenter(QPainter* painter, double x, double y,
                                 const QRect& screen_coords) {
    return DrawMapWithCenter(painter, QPointF(x, y), screen_coords);
  }
  virtual void DrawMapWithCenter(QPainter* painter, const QPointF& pos,
                                 const QRect& screen_coords) = 0;

  virtual void UpdateBlock(QPoint position) { Q_UNUSED(position); }
};

#endif  // VIEW_ABSTRACT_MAP_DRAWER_H_
