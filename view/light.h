#ifndef VIEW_LIGHT_H_
#define VIEW_LIGHT_H_

#include <array>
#include <cstdint>

class Light {
 public:
  static constexpr int kNChannels = 4;
  static constexpr int kRedIndex = 0;
  static constexpr int kGreenIndex = 1;
  static constexpr int kBlueIndex = 2;
  static constexpr int kSunIndex = 3;
  static constexpr int8_t kMaxLight = 100;
  static constexpr int8_t kMinLight = 0;
  static constexpr int kDecreaseFactor = 10;

  Light() = default;
  constexpr Light(int8_t red, int8_t green, int8_t blue, int8_t sun)
      : data_{red, green, blue, sun} {}
  constexpr explicit Light(std::array<int8_t, kNChannels> data)
      : data_(data) {}

  int8_t GetChannel(int index) const { return data_[index]; }
  int8_t GetRed() const { return data_[kRedIndex]; }
  int8_t GetGreen() const { return data_[kGreenIndex]; }
  int8_t GetBlue() const { return data_[kBlueIndex]; }
  int8_t GetSun() const { return data_[kSunIndex]; }

  void SetChannel(int index, int8_t value) { data_[index] = value; }
  void SetRed(int8_t value) { data_[kRedIndex] = value; }
  void SetGreen(int8_t value) { data_[kGreenIndex] = value; }
  void SetBlue(int8_t value) { data_[kBlueIndex] = value; }
  void SetSun(int8_t value) { data_[kSunIndex] = value; }

  Light GetNearLight() const;

  bool CanBeUpdated(const Light& light) const;
  bool IsDepended(const Light& light) const;
  bool IsDark() const;
  void Combine(const Light& light);
  void Reset();

 private:
  static float sun_intensivity_;

  std::array<int8_t, kNChannels> data_;
};

#endif  // VIEW_LIGHT_H_
