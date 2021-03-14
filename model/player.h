#ifndef MODEL_PLAYER_H_
#define MODEL_PLAYER_H_

#include <QPoint>

#include "model/constants.h"
#include "model/moving_object.h"

class Player : public MovingObject {
 public:
  explicit Player(QPointF pos);

 private:
  // TODO(Wind-Eagle): when jump off the block, acceleration now resets;
};

#endif  // MODEL_PLAYER_H_
