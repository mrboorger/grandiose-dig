#ifndef MODEL_CLEARABLE_CACHE_H_
#define MODEL_CLEARABLE_CACHE_H_

#include <QTimer>
#include <functional>
#include <map>
#include <utility>

namespace containers {

template<typename Key, typename Value, typename Compare = std::less<>>
class ClearableCache {
 public:
  explicit ClearableCache(int clear_time_msec, Compare compare = Compare())
      : nodes_(compare), last_used_(nodes_.end()) {
    timer_.callOnTimeout([this]() { ClearUnused(); });
    timer_.start(clear_time_msec);
  }

  Value& Insert(const Key& key, Value&& value) {
    return nodes_.emplace(key, Node{std::forward<Value>(value), true})
        .first->second.value;
  }

  std::optional<std::reference_wrapper<Value>> Get(const Key& key) {
    if (last_used_ == nodes_.end() || last_used_->first != key) {
      last_used_ = nodes_.find(key);
    }
    if (last_used_ == nodes_.end()) {
      return std::nullopt;
    }
    last_used_->second.is_used = true;
    return last_used_->second.value;
  }

  void ClearUnused() {
    last_used_ = nodes_.end();
    for (auto i = nodes_.begin(); i != nodes_.end();) {
      if (!i->second.is_used) {
        i = nodes_.erase(i);
      } else {
        i->second.is_used = false;
        ++i;
      }
    }
  }

 private:
  struct Node {
    Value value;
    bool is_used;
  };

  using NodesContainer = std::map<Key, Node, Compare>;
  using NodesIterator = typename NodesContainer::iterator;

  NodesContainer nodes_;
  QTimer timer_;
  NodesIterator last_used_;
};

}  // namespace containers

#endif  // MODEL_CLEARABLE_CACHE_H_
