#ifndef MODEL_PERLIN_NOISE1D_H_
#define MODEL_PERLIN_NOISE1D_H_

#include <QPoint>
#include <vector>

class PerlinNoise1D {
 public:
  static constexpr double kMin = -1.0;
  static constexpr double kMax = 1.0;

  explicit PerlinNoise1D(int seed, int grad_period = 1'000);

  // Returns noise in range [-1; 1]
  double operator()(double x) const;

 private:
  double Fade(double t) const;
  int Grad(double p) const;
  double Noise(double x) const;

  std::vector<int> grad_values_;
};

#endif  // MODEL_PERLIN_NOISE1D_H_
