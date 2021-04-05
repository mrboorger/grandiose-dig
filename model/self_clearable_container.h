#ifndef MODEL_SELF_CLEARABLE_CONTAINER_H_
#define MODEL_SELF_CLEARABLE_CONTAINER_H_

#include <QTimer>
#include <algorithm>
#include <utility>

#include "model/constants.h"

namespace containers {

template<typename T, int clear_time_msec, typename IsUsed, typename ClearUse>
class SelfClearableContainer : public T {
 public:
  explicit SelfClearableContainer(IsUsed is_used = IsUsed(),
                                  ClearUse clear_use = ClearUse())
      : T(),
        is_used_(std::move(is_used)),
        clear_use_(std::move(clear_use)) {
    SetupTimer();
  }

  template <class... Args>
  explicit SelfClearableContainer(Args... args,
                                  IsUsed is_used = IsUsed(),
                                  ClearUse clear_use = ClearUse())
      : T(std::forward(args...)),
        is_used_(std::move(is_used)),
        clear_use_(std::move(clear_use)) {
    SetupTimer();
  }

 private:
  using iterator = typename T::iterator;

  void ClearUsed() {
    for (auto i = T::begin(); i != T::end();) {
      if (!is_used_(i)) {
        i = T::erase(i);
      } else {
        clear_use_(i);
        ++i;
      }
    }
  }

  void SetupTimer() {
    clear_timer_.callOnTimeout([this]() { ClearUsed(); });
    clear_timer_.start(clear_time_msec);
  }

  QTimer clear_timer_;
  IsUsed is_used_;
  ClearUse clear_use_;
};

}  // namespace containers

#endif  // MODEL_SELF_CLEARABLE_CONTAINER_H_
