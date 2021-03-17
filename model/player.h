#ifndef MODEL_PLAYER_H_
#define MODEL_PLAYER_H_

#include <QPoint>

#include "model/moving_object.h"

class Player : public MovingObject {
 public:
  explicit Player(QPointF pos);
};

#endif  // MODEL_PLAYER_H_
