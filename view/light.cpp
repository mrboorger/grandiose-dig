#include "view/light.h"

#include <algorithm>

bool Light::CanBeUpdated(const Light& light) const {
  for (int i = 0; i < kNChannels; ++i) {
    if (light.data_[i] - kDecreaseFactor > data_[i]) {
      return true;
    }
  }
  return false;
}

bool Light::IsDepended(const Light& light) const {
  for (int i = 0; i < kNChannels; ++i) {
    if (light.data_[i] - kDecreaseFactor == data_[i]) {
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

void Light::Combine(const Light& light) {
  for (int i = 0; i < kNChannels; ++i) {
    data_[i] = std::max(1 * data_[i], light.data_[i] - kDecreaseFactor);
  }
}

void Light::GetMax(const Light& light) {
  for (int i = 0; i < kNChannels; ++i) {
    data_[i] = std::max(data_[i], light.data_[i]);
  }
}

void Light::Reset() {
  for (int i = 0; i < kNChannels; ++i) {
    data_[i] = kMinLight;
  }
}
