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

int MovingObjectDrawer::GetPictureNumber(
    int id, const std::shared_ptr<MovingObject>& object, int state_time) {
  if (object->GetState() == MovingObject::State::kWalk ||
      id == GetIdForPlayerAttack()) {
    return (state_time / kAnimation[id]) % kPictures[id];
  }
  return 0;
}

const QImage& MovingObjectDrawer::GetMovingObjectImage(
    int id, const std::shared_ptr<MovingObject>& object) {
  const QString& picture = ":/resources/textures/" + kNames[id];
  int state_time = (id != Mob::kTypesCount + 1)
                       ? std::floor(object->GetStateTime())
                       : constants::kPlayerAttackTime -
                             Model::GetInstance()->GetPlayer()->GetAttackTick();
  int picture_number = GetPictureNumber(id, object, state_time);
  if (images[id][picture_number].isNull()) {
    images[id][picture_number] =
        QImage(picture + QString::number(picture_number) + ".png");
    images_reversed[id][picture_number] =
        images[id][picture_number].mirrored(true, false);
  }
  if (object->GetDirection() == utils::Direction::kLeft) {
    return images_reversed[id][picture_number];
  } else {
    return images[id][picture_number];
  }
}

void MovingObjectDrawer::DrawMob(QPainter* painter, QPointF point,
                                 std::shared_ptr<Mob> mob) {
  painter->drawImage(point,
                     GetMovingObjectImage(GetIdForMob(mob->GetMobType()), mob));
}

void MovingObjectDrawer::DrawPlayer(QPainter* painter, QPointF point) {
  if (!Model::GetInstance()->GetPlayer()->IsAttackFinished()) {
    const QImage& player_attack_image = GetMovingObjectImage(
        GetIdForPlayerAttack(), Model::GetInstance()->GetPlayer());
    const QImage& player_image = GetMovingObjectImage(
        GetIdForPlayer(), Model::GetInstance()->GetPlayer());
    const QImage& player_image_cropped = player_image.copy(QRect(
        QPoint(0, player_attack_image.size().height()), player_image.size()));
    painter->drawImage(point, player_attack_image);
    painter->drawImage(point + QPoint(0, player_attack_image.size().height()),
                       player_image_cropped);
  } else {
    painter->drawImage(point,
                       GetMovingObjectImage(GetIdForPlayer(),
                                            Model::GetInstance()->GetPlayer()));
  }
}
