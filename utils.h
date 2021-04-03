#ifndef UTILS_H_
#define UTILS_H_

#include <QPoint>
#include <cassert>
#include <cstdint>
#include <utility>

namespace utils {

std::pair<int64_t, int64_t> ArithmeticalDivMod(int64_t a, int64_t b);

struct QPointCompare {
  bool operator()(QPoint lhs, QPoint rhs) const;
};

}  // namespace utils

#endif  // UTILS_H_
