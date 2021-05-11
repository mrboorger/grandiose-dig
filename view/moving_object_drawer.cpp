#include "view/moving_object_drawer.h"

#include <QString>
#include <array>

#include "model/constants.h"
#include "model/mob.h"
#include "model/model.h"

namespace {

std::array<QString, Mob::kTypesCount + 2> kNames = {
    "zombie", "zombie_lord", "quiox", "magic_quiox", "player", "player_attack"};

std::array<int, Mob::kTypesCount + 2> kAnimation = {
    constants::kZombieParameters.walk_animation_,
    constants::kZombieLordParameters.walk_animation_,
    constants::kQuioxParameters.walk_animation_,
    constants::kQuioxParameters.walk_animation_,
    constants::kPlayerWalkAnimation,
    constants::kPlayerAttackAnimation};

std::array<int, Mob::kTypesCount + 2> kPictures = {
    constants::kZombieParameters.walk_pictures_,
    constants::kZombieLordParameters.walk_pictures_,
    constants::kQuioxParameters.walk_pictures_,
    constants::kQuioxParameters.walk_pictures_,
    constants::kPlayerWalkPictures,
    constants::kPlayerAttackPictures};

std::array<std::array<QImage, constants::kMaxAnimationPictures>,
           Mob::kTypesCount + 2>
    images;
std::array<std::array<QImage, constants::kMaxAnimationPictures>,
           Mob::kTypesCount + 2>
    images_reversed;

}  // namespace

const QImage& MovingObjectDrawer::GetMovingObjectImage(
    int id, std::shared_ptr<MovingObject> object, QString name, bool reversed) {
  QString mob_picture = ":/resources/textures/" + name;
  int picture_number = 0;
  if (object->GetState() == MovingObject::State::kWalk ||
      id == Mob::kTypesCount + 1) {
    int animation = kAnimation[id];
    int pictures = kPictures[id];
    int state_time = std::floor(object->GetStateTime());
    if (id == Mob::kTypesCount + 1) {
      state_time = constants::kPlayerAttackTime -
                   Model::GetInstance()->GetPlayer()->GetAttackTick();
    }
    picture_number = (state_time / animation) % pictures;
  }
  if (images[id][picture_number].isNull()) {
    images[id][picture_number] =
        QImage(":/resources/textures/" + kNames[id] +
               QString::number(picture_number) + ".png");
    images_reversed[id][picture_number] =
        images[id][picture_number].mirrored(true, false);
  }
  if (reversed) {
    return images_reversed[id][picture_number];
  } else {
    return images[id][picture_number];
  }
}

void MovingObjectDrawer::DrawMob(QPainter* painter, QPointF point,
                                 std::shared_ptr<Mob> mob) {
  painter->drawImage(
      point, GetMovingObjectImage(
                 mob->GetId(), mob, kNames[static_cast<int>(mob->GetId())],
                 mob->GetDirection() == utils::Direction::kLeft));
}

void MovingObjectDrawer::DrawPlayer(QPainter* painter, QPointF point) {
  painter->drawImage(point, GetPlayerImage());
}

void MovingObjectDrawer::DrawPlayerAttack(QPainter* painter, QPointF point) {
  painter->drawImage(point, GetPlayerAttackImage());
}

const QImage& MovingObjectDrawer::GetPlayerImage() {
  return GetMovingObjectImage(
      Mob::kTypesCount, Model::GetInstance()->GetPlayer(),
      kNames[static_cast<int>(Mob::kTypesCount)],
      Model::GetInstance()->GetPlayer()->GetDirection() ==
          utils::Direction::kLeft);
}
const QImage& MovingObjectDrawer::GetPlayerAttackImage() {
  return GetMovingObjectImage(
      Mob::kTypesCount + 1, Model::GetInstance()->GetPlayer(),
      kNames[static_cast<int>(Mob::kTypesCount + 1)],
      Model::GetInstance()->GetPlayer()->GetDirection() ==
          utils::Direction::kLeft);
}
