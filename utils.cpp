#include "utils.h"

namespace utils {

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

QPointF DivideSegment(QPointF first, QPointF second, double percentage) {
  return first * percentage + second * (1.0 - percentage);
}

double GetRandomDouble(double left_bound, double right_bound) {
  double ans = random();
  return static_cast<double>(ans) / static_cast<double>(1LL << 32) /
             (right_bound - left_bound) +
         left_bound;
}

}  // namespace utils
