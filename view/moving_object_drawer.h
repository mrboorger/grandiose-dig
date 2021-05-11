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

  static const QImage& GetPlayerImage();
  static const QImage& GetPlayerAttackImage();

 private:
  static const QImage& GetMovingObjectImage(
      int id, std::shared_ptr<MovingObject> object, QString name,
      bool reversed);
};

#endif  // VIEW_MOVING_OBJECT_DRAWER_H_
