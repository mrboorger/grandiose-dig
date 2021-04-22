#include "model/perlin_noise1d.h"

#include <random>

#include "utils.h"

PerlinNoise1D::PerlinNoise1D(int seed, int grad_period)
    : grad_values_(grad_period) {
  std::mt19937 gen(seed);
  std::uniform_int_distribution<> distrib(0, 1);
  for (auto& grad : grad_values_) {
    grad = 1 - 2 * distrib(gen);
  }
}

double PerlinNoise1D::operator()(double x) const {
  return Noise(x * (1.0 / 300.0)) * 1.0 + Noise(x * (1.0 / 150.0)) * 0.5 +
         Noise(x * (1.0 / 75.0)) * 0.25 + Noise(x * (1.0 / 37.5)) * 0.0125;
}

double PerlinNoise1D::Fade(double t) const {
  return t * t * t * (t * (6.0 * t - 15.0) + 10.0);
}

int PerlinNoise1D::Grad(double p) const {
  p = std::fmod(p, grad_values_.size());
  if (p < 0) {
    p += grad_values_.size();
  }
  return grad_values_[static_cast<int>(p)];
}

double PerlinNoise1D::Noise(double p) const {
  double p0 = std::floor(p);
  double p1 = p0 + 1.0;
  double t = p - p0;
  double fade_t = Fade(t);
  double g0 = Grad(p0);
  double g1 = Grad(p1);
  return utils::DivideDouble(g1 * (p - p1), g0 * (p - p0), fade_t);
}
