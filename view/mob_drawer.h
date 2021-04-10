#ifndef VIEW_MOB_DRAWER_H_
#define VIEW_MOB_DRAWER_H_

#include <QPainter>
#include <QPointF>
#include <memory>

#include "model/mob.h"

class MobDrawer {
 public:
  static void DrawMob(QPainter* painter, QPointF point,
                      std::shared_ptr<Mob> mob);
};

#endif  // VIEW_MOB_DRAWER_H_