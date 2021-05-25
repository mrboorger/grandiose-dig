#ifndef MODEL_CLEARABLE_CACHE_H_
#define MODEL_CLEARABLE_CACHE_H_

#include <QCborValue>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QTimer>
#include <functional>
#include <map>
#include <mutex>
#include <utility>

namespace containers {

template <typename Key, typename Value>
class Nothing {
 public:
  void operator()(const QString& save_file, const Key& key,
                  const Value& value) const {
    Q_UNUSED(save_file);
    Q_UNUSED(key);
    Q_UNUSED(value);
  }
};

template <typename Key, typename Value, typename Compare = std::less<>,
          typename Save = Nothing<Key, Value>>
class ClearableCache {
 public:
  explicit ClearableCache(QString save_file, int clear_time_msec,
                          Compare compare = Compare(), Save save = Save())
      : nodes_(compare),
        save_(std::move(save)),
        save_file_(std::move(save_file)),
        last_used_(nodes_.end()) {
    timer_.callOnTimeout([this]() { ClearUnused(); });
    timer_.start(clear_time_msec);
  }

  Value& Insert(const Key& key, Value&& value) {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return nodes_.emplace(key, Node{std::forward<Value>(value), true})
        .first->second.value;
  }

  std::optional<std::reference_wrapper<Value>> Get(const Key& key) {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
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
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    last_used_ = nodes_.end();
    for (auto i = nodes_.begin(); i != nodes_.end();) {
      if (!i->second.is_used) {
        i = nodes_.erase(i);
      } else {
        save_(save_file_, i->first, i->second.value);
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
  Save save_;
  QString save_file_;
  QTimer timer_;
  NodesIterator last_used_;
  std::recursive_mutex mutex_;
};

}  // namespace containers

#endif  // MODEL_CLEARABLE_CACHE_H_
