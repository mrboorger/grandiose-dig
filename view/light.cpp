#include "view/light.h"

Light Light::GetNearLight() const {
  Light result;
  for (int i = 0; i < kNChannels; ++i) {
    result.SetChannel(i, std::max(data_[i] - kDecreaseFactor,
                                  static_cast<int>(kDecreaseFactor)));
  }
  return result;
}

void Light::Combine(const Light& light) {
  for (int i = 0; i < kNChannels; ++i) {
    data_[i] = std::max(data_[i], light.GetChannel(i));
  }
}

void Light::Reset() {
  for (int i = 0; i < kNChannels; ++i) {
    data_[i] = kMinLight;
  }
}
