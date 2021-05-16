#include "utils.h"

#include <chrono>

namespace utils {

std::mt19937 random(
    std::chrono::steady_clock::now().time_since_epoch().count());

std::pair<int64_t, int64_t> ArithmeticalDivMod(int64_t a, int64_t b) {
  assert(b > 0);
  if (a > 0) {
    return std::make_pair(a / b, a % b);
  }
  return std::make_pair((a - (b - 1)) / b, ArithmeticalMod(a, b));
}

int64_t ArithmeticalMod(int64_t a, int64_t b) {
  auto mod = a % b;
  if (mod < 0) {
    mod += b;
  }
  return mod;
}

bool QPointLexicographicalCompare::operator()(QPoint lhs, QPoint rhs) const {
  if (lhs.x() != rhs.x()) {
    return lhs.x() < rhs.x();
  }
  return lhs.y() < rhs.y();
}

double DivideDouble(double first, double second, double percentage) {
  return first * percentage + second * (1.0 - percentage);
}

QPointF DivideSegment(QPointF first, QPointF second, double percentage) {
  return first * percentage + second * (1.0 - percentage);
}

double GetRandomDouble(double left_bound, double right_bound) {
  std::uniform_real_distribution<double> distrib(left_bound, right_bound);
  return distrib(random);
}

std::array<QPoint, 4> PointNeighbours(QPoint point) {
  return {QPoint(point.x(), point.y() - 1), QPoint(point.x(), point.y() + 1),
          QPoint(point.x() - 1, point.y()), QPoint(point.x() + 1, point.y())};
}

}  // namespace utils
