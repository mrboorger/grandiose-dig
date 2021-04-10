#ifndef MODEL_CLEARABLE_MAP_H_
#define MODEL_CLEARABLE_MAP_H_

#include <functional>
#include <map>
#include <memory>
#include <utility>

#include "model/self_clearable_container.h"

namespace containers {

template <typename T>
struct MapIsUsed {
  using iterator = typename T::iterator;

  bool operator()(const iterator& i) {
    return i->second.is_used;
  }
};

template <typename T>
struct MapClearUse {
  using iterator = typename T::iterator;

  void operator()(iterator i) {
    i->second.is_used = false;
  }
};

template<typename Key, typename T, typename Compare = std::less<Key>,
         int clear_time_msec = constants::kDefaultClearTimeMSec,
         typename Allocator = std::allocator<std::pair<const Key, T>>>
using ClearableMap =
    SelfClearableContainer<std::map<Key, T, Compare, Allocator>,
                           clear_time_msec,
                           MapIsUsed<std::map<Key, T, Compare, Allocator>>,
                           MapClearUse<std::map<Key, T, Compare, Allocator>>>;

}  // namespace containers

#endif  // MODEL_CLEARABLE_MAP_H_
