#ifndef VIEW_SOUND_MANAGER_H_
#define VIEW_SOUND_MANAGER_H_

#include <QMediaPlayer>
#include <memory>
#include <vector>

class SoundManager {
 public:
  enum class Sound {
    kPlayerDamage,
    kZombieDamage1,
    kZombieDamage2,
    kZombieDeath,
    kZombie1,
    kZombie2,
    kMusic1,
    kSoundsCount
  };

  static constexpr int kSoundsCount = static_cast<int>(Sound::kSoundsCount);

  SoundManager();
  void PlaySound(Sound sound) { sounds_[static_cast<int>(sound)]->play(); }
  void PauseSound(Sound sound) { sounds_[static_cast<int>(sound)]->pause(); }
  void StopSound(Sound sound) { sounds_[static_cast<int>(sound)]->stop(); }
  void PauseAllSounds();
  void StopAllSounds();

 private:
  std::vector<std::shared_ptr<QMediaPlayer>> sounds_;
};

#endif  // VIEW_SOUND_MANAGER_H_
