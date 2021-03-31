#ifndef UTILS_H_
#define UTILS_H_

#include <cassert>
#include <cstdint>
#include <utility>

namespace utils {

std::pair<int64_t, int64_t> ArithmeticalDivMod(int64_t a, int64_t b) {
  assert(b > 0);
  if (a > 0) {
    return std::make_pair(a / b, a % b);
  }
  a -= b - 1;
  auto mod = a % b;
  if (mod < 0) {
    mod += b;
  }
  return std::make_pair(a / b, mod);
}

}  // namespace utils

#endif  // UTILS_H_
