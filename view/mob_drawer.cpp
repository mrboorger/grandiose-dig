#include "view/mob_drawer.h"

#include <QString>
#include <array>

#include "model/constants.h"
#include "model/mob.h"

namespace {

std::array<QString, Mob::kTypesCount> kNames = {"zombie", "zombie_lord",
                                                "quiox", "magic_quiox"};

}  // namespace

QString MobDrawer::GetMobImage(std::shared_ptr<Mob> mob, QString name) {
  QString mob_picture = ":/resources/textures/" + name;
  if (mob->GetState() != MovingObject::State::kWalk) {
    return mob_picture + "0.png";
  }
  Mob::Type type = mob->GetMobType();
  int walk_animation = 100;
  int walk_pictures = 1;
  switch (type) {
    case Mob::Type::kZombie:
      walk_animation = constants::kZombieWalkAnimation;
      walk_pictures = constants::kZombieWalkPictures;
      break;
    case Mob::Type::kZombieLord:
      walk_animation = constants::kZombieLordWalkAnimation;
      walk_pictures = constants::kZombieLordWalkPictures;
      break;
    case Mob::Type::kQuiox:
      walk_animation = constants::kQuioxWalkAnimation;
      walk_pictures = constants::kQuioxWalkPictures;
      break;
    case Mob::Type::kMagicQuiox:
      walk_animation = constants::kQuioxWalkAnimation;
      walk_pictures = constants::kQuioxWalkPictures;
      break;
    default:
      break;
  }

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
