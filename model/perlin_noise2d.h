#ifndef MODEL_PERLIN_NOISE2D_H_
#define MODEL_PERLIN_NOISE2D_H_

#include <QPointF>
#include <vector>

class PerlinNoise2D {
 public:
  explicit PerlinNoise2D(int seed, int grad_period = 1'000);

  double operator()(double x, double y) const;

 private:
  double Fade(double t) const;
  QPointF Grad(QPointF p) const;
  double Noise(QPointF p) const;

  int grad_size_;
  std::vector<QPointF> grad_values_;
};

#endif  // MODEL_PERLIN_NOISE2D_H_
