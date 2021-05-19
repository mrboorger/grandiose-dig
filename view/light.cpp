#include "view/light.h"

#include <algorithm>

bool Light::IsDepenantOn(const Light& light) const {
  for (int i = 0; i < kNChannels; ++i) {
    if (static_cast<int>(light.data_[i]) - kDecreaseFactor == data_[i]) {
      return true;
    }
  }
  return false;
}

bool Light::IsDark() const {
  for (int i = 0; i < kNChannels; ++i) {
    if (data_[i] > kMinLight) {
      return false;
    }
  }
  return true;
}

bool Light::Combine(const Light& light) {
  bool result = false;
  for (int i = 0; i < kNChannels; ++i) {
    if (int new_light = static_cast<int>(light.data_[i]) - kDecreaseFactor;
        new_light > data_[i]) {
      result = true;
      data_[i] = new_light;
    }
  }
  return result;
}

void Light::UpdateMax(const Light& light) {
  for (int i = 0; i < kNChannels; ++i) {
    data_[i] = std::max(data_[i], light.data_[i]);
  }
}

void Light::Reset() {
  for (int i = 0; i < kNChannels; ++i) {
    data_[i] = kMinLight;
  }
}
