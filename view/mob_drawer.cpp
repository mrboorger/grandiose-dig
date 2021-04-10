#include "view/mob_drawer.h"

#include <QString>
#include <array>

#include "model/constants.h"
#include "model/mob.h"

namespace {

std::array<QString, Mob::kTypesCount> names = {"mob.png"};
std::array<QImage, Mob::kTypesCount> images;

}  // namespace

void MobDrawer::DrawMob(QPainter* painter, QPointF point,
                        std::shared_ptr<Mob> mob) {
  if (images[mob->GetId()].isNull()) {
    images[mob->GetId()] =
        QImage(":/resources/textures/" + names[mob->GetId()]);
  }
  painter->drawImage(point, images[mob->GetId()]);
}
