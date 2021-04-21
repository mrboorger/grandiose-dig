#ifndef UTILS_H_
#define UTILS_H_

#include <QPoint>
#include <QPointF>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <ctime>
#include <random>
#include <utility>

namespace utils {

std::pair<int64_t, int64_t> ArithmeticalDivMod(int64_t a, int64_t b);

int64_t ArithmeticalMod(int64_t a, int64_t b);

struct QPointLexicographicalCompare {
  bool operator()(QPoint lhs, QPoint rhs) const;
};

constexpr double MapRange(double value, double from_min, double from_max,
                          double to_min, double to_max) {
  return to_min +
         (value - from_min) / (from_max - from_min) * (to_max - to_min);
}

QPointF DivideSegment(QPointF first, QPointF second, double percentage);

static std::mt19937 random(time(NULL));

double GetRandomDouble(double left_bound = 0, double right_bound = 1);

}  // namespace utils

#endif  // UTILS_H_
