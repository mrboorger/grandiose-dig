#ifndef VIEW_MOVING_OBJECT_DRAWER_H_
#define VIEW_MOVING_OBJECT_DRAWER_H_

#include <QPainter>
#include <QPointF>
#include <memory>

#include "model/mob.h"
#include "model/player.h"

class MovingObjectDrawer {
 public:
  static void DrawMob(QPainter* painter, QPointF point,
                      std::shared_ptr<Mob> mob);
  static void DrawPlayer(QPainter* painter, QPointF point);
  static void DrawPlayerAttack(QPainter* painter, QPointF point);

 private:
  static int GetIdForMob(Mob::Type type) { return static_cast<int>(type); }
  static int GetIdForPlayer() { return Mob::kTypesCount; }
  static int GetIdForPlayerAttack() { return Mob::kTypesCount + 1; }
  static const QImage& GetMovingObjectImage(
      int id, const std::shared_ptr<MovingObject>& object);
  static int GetPictureNumber(int id,
                              const std::shared_ptr<MovingObject>& object,
                              int state_time);
};

#endif  // VIEW_MOVING_OBJECT_DRAWER_H_
