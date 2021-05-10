#include "view/mob_drawer.h"

#include <QString>
#include <array>

#include "model/constants.h"
#include "model/mob.h"

namespace {

std::array<QString, Mob::kTypesCount> kNames = {"zombie", "zombie_lord",
                                                "quiox", "magic_quiox"};

std::array<MobParameters, Mob::kTypesCount> kMobParameters = {
    constants::kZombieParameters, constants::kZombieLordParameters,
    constants::kQuioxParameters, constants::kQuioxParameters};

}  // namespace

QString MobDrawer::GetMobImage(std::shared_ptr<Mob> mob, QString name) {
  QString mob_picture = ":/resources/textures/" + name;
  if (mob->GetState() != MovingObject::State::kWalk) {
    return mob_picture + "0.png";
  }
  Mob::Type type = mob->GetMobType();
  int walk_animation = kMobParameters[static_cast<int>(type)].walk_animation_;
  int walk_pictures = kMobParameters[static_cast<int>(type)].walk_pictures_;
  int state_time = std::floor(mob->GetStateTime());
  int picture_number = (state_time / walk_animation) % walk_pictures;
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
