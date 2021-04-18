#include "model/perlin_noise2d.h"

#include <cmath>
#include <random>

#include "model/constants.h"

PerlinNoise2D::PerlinNoise2D(int seed, int grad_period)
    : grad_size_(grad_period), grad_values_(grad_period * grad_period) {
  std::mt19937 gen(seed);
  std::uniform_real_distribution<> distrib(0.0, 1.0);
  for (auto& j : grad_values_) {
    double l = 0.0;
    do {
      j = QPointF(distrib(gen), distrib(gen));
      j = 2.0 * j - QPointF(1.0, 1.0);
      l = std::hypot(j.x(), j.y());
      if (l > constants::kEps) {
        j /= l;
      }
    } while (l <= constants::kEps);
  }
}

double PerlinNoise2D::operator()(double x, double y) const {
  QPointF p(x, y);
  return Noise(p * (1.0 / 300.0)) * 1.0 + Noise(p * (1.0 / 150.0)) * 0.5 +
         Noise(p * (1.0 / 75.0)) * 0.25 + Noise(p * (1.0 / 37.5)) * 0.0125;
}

double PerlinNoise2D::Fade(double t) const {
  return t * t * t * (t * (6.0 * t - 15.0) + 10.0);
}

QPointF PerlinNoise2D::Grad(QPointF p) const {
  auto normalize = [this](double x) {
    x = std::fmod(x, grad_size_);
    if (x < -constants::kEps) {
      x += grad_size_;
    }
    return static_cast<int>(std::round(x));
  };
  return grad_values_[normalize(p.y()) * grad_size_ + normalize(p.x())];
}

double PerlinNoise2D::Noise(QPointF p) const {
  QPointF p0 = QPointF(std::floor(p.x()), std::floor(p.y()));
  QPointF p1 = p0 + QPointF(1.0, 0.0);
  QPointF p2 = p0 + QPointF(0.0, 1.0);
  QPointF p3 = p0 + QPointF(1.0, 1.0);

  QPointF g0 = Grad(p0);
  QPointF g1 = Grad(p1);
  QPointF g2 = Grad(p2);
  QPointF g3 = Grad(p3);

  double t0 = p.x() - p0.x();
  double fade_t0 = Fade(t0);

  double t1 = p.y() - p0.y();
  double fade_t1 = Fade(t1);

  auto dot = [](QPointF a, QPointF b) { return a.x() * b.x() + a.y() * b.y(); };

  double p0p1 =
      (1.0 - fade_t0) * dot(g0, (p - p0)) + fade_t0 * dot(g1, (p - p1));
  double p2p3 =
      (1.0 - fade_t0) * dot(g2, (p - p2)) + fade_t0 * dot(g3, (p - p3));

  return (1.0 - fade_t1) * p0p1 + fade_t1 * p2p3;
}
