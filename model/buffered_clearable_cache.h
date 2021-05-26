#ifndef MODEL_BUFFERED_CLEARABLE_CACHE_H_
#define MODEL_BUFFERED_CLEARABLE_CACHE_H_

#include <QPoint>
#include <QRect>
#include <array>
#include <functional>
#include <optional>
#include <utility>

#include "model/clearable_cache.h"
#include "model/constants.h"
#include "utils.h"

namespace containers {

template <typename T>
class GenEmptyBuffer {
 public:
  T operator()(QPoint buffer_pos) const {
    Q_UNUSED(buffer_pos);
    return T{};
  }
};

template <typename T, int32_t width, int32_t height,
          typename BufferType = std::array<T, width * height>,
          typename F = GenEmptyBuffer<BufferType>>
class BufferedClearableCache {
 public:
  using Buffer = BufferType;

  static constexpr int32_t kBufferWidth = width;
  static constexpr int32_t kBufferHeight = height;

  static std::pair<QPoint, QPoint> RoundToBufferPos(QPoint p) {
    QPoint local_pos = QPoint(utils::ArithmeticalMod(p.x(), width),
                              utils::ArithmeticalMod(p.y(), height));
    return std::pair(p - local_pos, local_pos);
  }

  static int32_t BufferIndex(QPoint local_pos) {
    return local_pos.y() * width + local_pos.x();
  }

  BufferedClearableCache(int clear_time_msec = constants::kDefaultClearTimeMSec,
                         F gen_buffer = F())
      : data_(clear_time_msec), gen_buffer_(std::move(gen_buffer)) {}

  std::optional<std::reference_wrapper<const T>> GetValueOpt(QPoint pos) {
    auto [buffer_pos, local_pos] = RoundToBufferPos(pos);
    auto found = data_.Get(buffer_pos);
    if (!found) {
      return std::nullopt;
    }
    const Buffer& buffer = found.value();
    const T& value = buffer[BufferIndex(local_pos)];
    return value;
  }

  std::optional<T*> GetMutableValueOpt(QPoint pos) {
    auto result = GetValueOpt(pos);
    if (!result) {
      return std::nullopt;
    }
    return const_cast<T*>(&result.value());
  }

  const T& GetValue(QPoint pos) {
    auto [buffer_pos, local_pos] = RoundToBufferPos(pos);
    return GetOrInsertBuffer(buffer_pos)[BufferIndex(local_pos)];
  }

  T* GetMutableValue(QPoint pos) { return const_cast<T*>(&GetValue(pos)); }

  template <typename V>
  T& SetValue(QPoint pos, V&& value) {
    auto [buffer_pos, local_pos] = RoundToBufferPos(pos);
    return GetOrInsertBuffer(buffer_pos)[BufferIndex(local_pos)] =
               std::forward<T>(value);
  }

  Buffer& GetOrInsertBufferPos(QPoint pos) {
    QPoint buffer_pos = RoundToBufferPos(pos).first;
    return GetOrInsertBuffer(buffer_pos);
  }

  void MarkUsedOrInsert(const QRect& rect) {
    QPoint begin = RoundToBufferPos(rect.topLeft()).first;
    QPoint end = RoundToBufferPos(rect.bottomRight()).first;
    for (int32_t y = begin.y(); y <= end.y(); y += height) {
      for (int32_t x = begin.x(); x <= end.x(); x += width) {
        GetOrInsertBuffer(QPoint(x, y));
      }
    }
  }

 private:
  Buffer& GetOrInsertBuffer(QPoint buffer_pos) {
    auto found = data_.Get(buffer_pos);
    if (!found) {
      return data_.Insert(buffer_pos, gen_buffer_(buffer_pos));
    }
    return found.value();
  }

  ClearableCache<QPoint, Buffer, utils::QPointLexicographicalCompare> data_;
  F gen_buffer_;
};

}  // namespace containers

#endif  // MODEL_BUFFERED_CLEARABLE_CACHE_H_
