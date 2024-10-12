#pragma once

#include <cstdint>
#include <queue>
#include <vector>

#include <Logger/logger.hpp>
#include <Utils/moving_average.hpp>

namespace btc2::io {

using KeyboardKeyType = int32_t;
using DelayMsType = int32_t;

class KeySequenceElementKind {
 public:
  enum Kind { None = 0, Key = 1, Delay = 2 };
};

template <typename T>
class KeySequenceElement {
 public:
  using Kind = KeySequenceElementKind::Kind;

 public:
  KeySequenceElement(T key, bool key_pressed) : m_kind{Kind::Key}, m_key{key}, m_key_pressed{key_pressed} {
    SPDLOG_DEBUG("Built key ({}) pressed: {}", m_key, m_key_pressed);
  }
  KeySequenceElement(DelayMsType delay) : m_kind{Kind::Delay}, m_delay_ms{delay} {
    SPDLOG_DEBUG("Built delay ({}ms)", m_delay_ms);
  }
  KeySequenceElement() = default;

  auto GetKind() const {
    return m_kind;
  }

  const T& GetKey() const {
    return m_key;
  }
  bool GetKeyPressed() const {
    return m_key_pressed;
  }
  DelayMsType GetDelayMs() const {
    return m_delay_ms;
  }

 private:
  Kind m_kind;
  T m_key{};
  bool m_key_pressed{};
  DelayMsType m_delay_ms{};
};

struct KeySequence {
 public:
  KeySequence(std::initializer_list<KeySequenceElement<KeyboardKeyType>> seq) : m_seq{seq} {}

  auto ToVector() const {
    std::vector<KeySequenceElement<KeyboardKeyType>> vec{};
    vec.reserve(m_seq.size());
    auto tmp{m_seq};
    while (!tmp.empty()) {
      vec.push_back(tmp.front());
      tmp.pop();
    }
    return vec;
  }

  KeySequenceElement<KeyboardKeyType> Pop() {
    if (m_seq.empty()) {
      return {0}; /* Return a null delay if the queue is empty. Definitely not meant to be used that way */
    }
    auto e{m_seq.front()};
    m_seq.pop();
    return e;
  }

  bool Empty() const {
    return m_seq.empty();
  }

  void AddKey(KeySequenceElement<KeyboardKeyType> e) {
    m_seq.push(std::move(e));
  }

 private:
  std::queue<KeySequenceElement<KeyboardKeyType>> m_seq{};
};

void AsynchronousKeySeqOld(const KeySequence& seq);
void AsynchronousKeySeqThread(const KeySequence& seq);

class KeySequencerThread {
 public:
  ~KeySequencerThread();

  static auto& I() {
    static KeySequencerThread thread{};
    return thread;
  }

  void Run();
  static void Stop();
  void Loop();

  void SetSequence(const KeySequence& seq);

  bool IsASequenceRunning() {
    std::scoped_lock lock{m_mutex};
    return !m_seq.Empty();
  }

 private:
  KeySequencerThread() = default;
  using TimePoint = std::chrono::high_resolution_clock::time_point;
  static TimePoint Now() {
    return std::chrono::high_resolution_clock::now();
  }

 private:
  std::atomic_bool m_continue{true};

  std::mutex m_mutex{};

  KeySequence m_seq{};
  TimePoint m_seq_set_time{};
  TimePoint m_seq_finished_time{};

  std::optional<KeySequenceElement<KeyboardKeyType>> m_current_element{};
  TimePoint m_delay_start{};
  bool m_last_loop_was_empty{true};

  utils::MovingAverage<double> m_avg_seq_time{10};
};

}  // namespace btc2::io
