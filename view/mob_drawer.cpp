#include "view/mob_drawer.h"

#include <QDebug>
#include <QString>
#include <array>

#include "model/constants.h"
#include "model/mob.h"

namespace {

std::array<QString, Mob::kTypesCount> kNames = {"mob"};

}  // namespace

QString MobDrawer::GetMobImage(std::shared_ptr<Mob> mob, QString name) {
  QString mob_picture = ":/resources/textures/" + name;
  if (mob->GetState() != MovingObject::State::kWalk) {
    return mob_picture + "0.png";
  }
  int state_time = std::floor(mob->GetStateTime());
  int picture_number =
      (state_time / constants::kMobWalkAnimation) % constants::kMobWalkPictures;
  return mob_picture + QString::number(picture_number) + ".png";
}

void MobDrawer::DrawMob(QPainter* painter, QPointF point,
                        std::shared_ptr<Mob> mob) {
  QImage mob_image(GetMobImage(mob, kNames[static_cast<int>(mob->GetId())]));
  if (mob->GetDirection() == utils::Direction::kLeft) {
    mob_image = mob_image.mirrored(true, false);
  }
  painter->drawImage(point, mob_image);
}
