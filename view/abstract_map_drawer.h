#ifndef VIEW_ABSTRACT_MAP_DRAWER_H_
#define VIEW_ABSTRACT_MAP_DRAWER_H_

#include <QPainter>
#include <QPointF>
#include <QRect>
#include <mutex>

class AbstractMapDrawer {
 public:
  AbstractMapDrawer() = default;
  AbstractMapDrawer(const AbstractMapDrawer&) = delete;
  AbstractMapDrawer(AbstractMapDrawer&&) = delete;

  virtual ~AbstractMapDrawer() = default;

  AbstractMapDrawer& operator=(const AbstractMapDrawer&) = delete;
  AbstractMapDrawer& operator=(AbstractMapDrawer&&) = delete;

  void Init() {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    InitImpl();
  }

  void DrawMapWithCenter(QPainter* painter, const QPointF& pos,
                                 const QRect& screen_coords) {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    DrawMapWithCenterImpl(painter, pos, screen_coords);
  }

  QRect GetDrawRegion(QPoint center) {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return GetDrawRegionImpl(center);
  }

  void UpdateBlock(QPoint position) {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    UpdateBlockImpl(position);
  }

 protected:
  virtual void InitImpl() {}
  virtual void DrawMapWithCenterImpl(QPainter* painter, const QPointF& pos,
                                 const QRect& screen_coords) = 0;
  virtual QRect GetDrawRegionImpl(QPoint center) const = 0;

  virtual void UpdateBlockImpl(QPoint position) { Q_UNUSED(position); }

 private:
  std::recursive_mutex mutex_;
};

#endif  // VIEW_ABSTRACT_MAP_DRAWER_H_
