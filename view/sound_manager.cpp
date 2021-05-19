#include "sound_manager.h"

#include <QSettings>

namespace {

const std::vector<QString> kNames = {"player_damage.mp3",
                                     "zombie1.mp3",
                                     "zombie2.mp3",
                                     "zombie_damage1.mp3",
                                     "zombie_damage2.mp3",
                                     "zombie_death.mp3",
                                     "zombie_lord1.mp3",
                                     "zombie_lord2.mp3",
                                     "zombie_lord_damage1.mp3",
                                     "zombie_lord_damage2.mp3",
                                     "zombie_lord_death.mp3",
                                     "quiox1.mp3",
                                     "quiox2.mp3",
                                     "quiox_damage1.mp3",
                                     "quiox_damage2.mp3",
                                     "quiox_death.mp3",
                                     "magic_quiox1.mp3",
                                     "magic_quiox2.mp3",
                                     "magic_quiox_damage1.mp3",
                                     "magic_quiox_damage2.mp3",
                                     "magic_quiox_death.mp3",
                                     "music.mp3"};

}  // namespace

SoundManager::SoundManager() : sounds_(kNames.size()) {
  for (size_t i = 0; i < kNames.size(); i++) {
    sounds_[i] = std::make_shared<QMediaPlayer>();
    sounds_[i]->setMedia(QUrl("qrc:/resources/sounds/" + kNames[i]));
  }
  UpdateVolumes();
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

int SoundManager::SoundIndex(Sound sound, int mob_id, MobSound mob_sound) {
  switch (sound) {
    case Sound::kPlayerDamage: {
      return 0;
    }
    case Sound::kMob: {
      return mob_id * static_cast<int>(MobSound::kMobSoundsCount) +
             static_cast<int>(mob_sound) + 1;
    }
    case Sound::kMusic: {
      return static_cast<int>(kNames.size()) - 1;
    }
    default:
      assert(false);
  }
}

bool SoundManager::IsMusic(const QString& name) {
  return name.startsWith("music");
}

void SoundManager::UpdateVolumes() {
  QSettings settings;
  int general_volume = settings.value("general_volume", 100).toInt();
  int music_volume = settings.value("music_volume", 100).toInt();
  int sounds_volume = settings.value("sounds_volume", 100).toInt();
  for (size_t i = 0; i < kNames.size(); i++) {
    int volume =
        general_volume * (IsMusic(kNames[i]) ? music_volume : sounds_volume);
    volume = volume * volume / 1000000;
    sounds_[i]->setVolume(volume);
  }
}
