#ifndef VIEW_LIGHT_H_
#define VIEW_LIGHT_H_

#include <QJsonArray>
#include <QJsonObject>
#include <algorithm>
#include <array>
#include <cstdint>

class Light {
 public:
  static constexpr int kNChannels = 4;
  static constexpr int kRedIndex = 0;
  static constexpr int kGreenIndex = 1;
  static constexpr int kBlueIndex = 2;
  static constexpr int kSunIndex = 3;
  static constexpr uint8_t kMaxLight = 255;
  static constexpr uint8_t kMinLight = 0;
  static constexpr uint8_t kDecreaseFactor = 10;

  Light() = default;
  constexpr Light(uint8_t red, uint8_t green, uint8_t blue, uint8_t sun)
      : data_{red, green, blue, sun} {}
  constexpr explicit Light(std::array<uint8_t, kNChannels> data)
      : data_(data) {}

  uint8_t GetChannel(int index) const { return data_[index]; }
  uint8_t GetRed() const { return data_[kRedIndex]; }
  uint8_t GetGreen() const { return data_[kGreenIndex]; }
  uint8_t GetBlue() const { return data_[kBlueIndex]; }
  uint8_t GetSun() const { return data_[kSunIndex]; }
  uint8_t GetCombinedLight() const {
    return std::max(0.30 * data_[kRedIndex] + 0.59 * data_[kGreenIndex] +
                        0.11 * data_[kBlueIndex],
                    data_[kSunIndex] * 1.0);
  }

  void SetChannel(int index, uint8_t value) { data_[index] = value; }
  void SetRed(uint8_t value) { data_[kRedIndex] = value; }
  void SetGreen(uint8_t value) { data_[kGreenIndex] = value; }
  void SetBlue(uint8_t value) { data_[kBlueIndex] = value; }
  void SetSun(uint8_t value) { data_[kSunIndex] = value; }

  bool IsDepenantOn(const Light& light) const;
  bool IsDark() const;
  bool Combine(const Light& light);
  void UpdateMax(const Light& light);
  void Reset();

  void Read(const QJsonObject& json);
  void Write(QJsonObject* json) const;

 private:
  std::array<uint8_t, kNChannels> data_;
};

#endif  // VIEW_LIGHT_H_
