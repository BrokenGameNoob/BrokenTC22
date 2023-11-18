#pragma once

#include <chrono>

#include <Logger/logger.hpp>

namespace btc2 {

using TimePoint = decltype(std::chrono::high_resolution_clock::now());

inline TimePoint NowMs() {
  return std::chrono::high_resolution_clock::now();
}

inline auto DiffMs(TimePoint start, TimePoint end) {
  return std::chrono::duration<double>(end - start).count() * 1000;
}

inline void PrintTimeDiff(const QString& id, TimePoint start, TimePoint end) {
  SPDLOG_DEBUG("[{}] Had a duration of {}ms", id, DiffMs(start, end));
}

}  // namespace btc2
