#include "sound_manager.h"

namespace {

const std::array<QString, SoundManager::kSoundsCount> names = {
    "player_damage.mp3", "zombie_damage1.mp3", "zombie_damage2.mp3",
    "zombie_death.mp3",  "zombie1.mp3",        "zombie2.mp3"};

}  // namespace

SoundManager::SoundManager() {
  sounds_.resize(kSoundsCount);
  for (int i = 0; i < kSoundsCount; i++) {
    sounds_[i] = std::make_shared<QMediaPlayer>();
    sounds_[i]->setMedia(QUrl("qrc:/resources/sounds/" + names[i]));
    sounds_[i]->setVolume(100);
  }
}

void SoundManager::PauseAllSounds() {
  for (int i = 0; i < kSoundsCount; i++) {
    sounds_[i]->pause();
  }
}
void SoundManager::StopAllSounds() {
  for (int i = 0; i < kSoundsCount; i++) {
    sounds_[i]->stop();
  }
}
