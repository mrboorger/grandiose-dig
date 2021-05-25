#ifndef VIEW_MAP_DRAWER_H_
#define VIEW_MAP_DRAWER_H_

#include <memory>

#include "model/abstract_map.h"
#include "view/abstract_map_drawer.h"

class MapDrawer : public AbstractMapDrawer {
 public:
  explicit MapDrawer(std::shared_ptr<AbstractMap> map);

 private:
  void DrawMapWithCenterImpl(QPainter* painter, const QPointF& pos,
                         const QRect& screen_coords) override;


  QRect GetDrawRegionImpl(QPoint center) const override;
  static constexpr int kFieldOfView = 100;
  std::shared_ptr<AbstractMap> map_;
};

#endif  // VIEW_MAP_DRAWER_H_
