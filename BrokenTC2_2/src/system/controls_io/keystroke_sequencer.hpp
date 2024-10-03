#pragma once

#include <cstdint>
#include <type_traits>
#include <vector>

#include <Logger/logger.hpp>

namespace btc2::io {

using KeyboardKeyType = int32_t;
using DelayMsType = int32_t;

class KeySequenceElementKind {
 public:
  enum Kind { Key = 0, Delay = 1 };
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

using KeySequence = std::vector<KeySequenceElement<KeyboardKeyType>>;

void AsynchronousKeySeq(const KeySequence& seq);

class KeySequencerThread {
 public:
  ~KeySequencerThread();

  static auto& I(){
    static KeySequencerThread thread{};
    return thread;
  }

  void Run();
  static void Stop();
  void Loop();

 private:
  std::atomic_bool m_continue{true};
};

}  // namespace btc2::io
