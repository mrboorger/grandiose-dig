#ifndef VIEW_SOUND_MANAGER_H_
#define VIEW_SOUND_MANAGER_H_

#include <QMediaPlayer>
#include <memory>
#include <vector>

class SoundManager {
 public:
  enum class Sound { kPlayerDamage, kMob, kMusic, kSoundsCount };
  enum class MobSound {
    kIdle1,
    kIdle2,
    kDamage1,
    kDamage2,
    kDeath,
    kMobSoundsCount
  };

  static constexpr int kSoundsCount = static_cast<int>(Sound::kSoundsCount);
  static constexpr int kMobSoundsCount =
      static_cast<int>(MobSound::kMobSoundsCount);

  SoundManager();
  void PlaySound(int id, int volume = 100) {
    {
      sounds_[id]->pause();
      sounds_[id]->setVolume(sounds_[id]->volume() * volume / 100);
      sounds_[id]->play();
    }
  }
  void PauseSound(int id) { sounds_[id]->pause(); }
  void StopSound(int id) { sounds_[id]->stop(); }
  static int SoundIndex(Sound sound, int mob_id = 0,
                        MobSound mob_sound = MobSound::kIdle1);
  void PauseAllSounds();
  void StopAllSounds();

  void UpdateVolumes(int general_volume, int music_volume, int sounds_volume);

  bool MusicIsPlaying() {
    return sounds_[SoundIndex(Sound::kMusic)]->state() ==
           QMediaPlayer::PlayingState;
  }

 private:
  static bool IsMusic(const QString& name);

  std::vector<std::shared_ptr<QMediaPlayer>> sounds_;
};

#endif  // VIEW_SOUND_MANAGER_H_
