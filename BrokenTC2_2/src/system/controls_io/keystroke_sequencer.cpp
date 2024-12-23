#include "keystroke_sequencer.hpp"

#include <QtConcurrent>

#include <Logger/logger.hpp>
#include <WinUtils/event_handler.hpp>
#include <utils/time.hpp>

namespace btc2::io {

void AsynchronousKeySeqOld(const KeySequence& seq_i) {
  auto seq{seq_i.ToVector()};
  const auto kBeforeStart{NowMs()};
  std::ignore = QtConcurrent::run([seq, kBeforeStart]() {
    for (const auto& e : seq) {
      switch (e.GetKind()) {
        case KeySequenceElementKind::Key: {
          win::SendKeyboardEvent(e.GetKey(), e.GetKeyPressed());
        } break;
        case KeySequenceElementKind::Delay: {
          QThread::msleep(e.GetDelayMs());
        } break;
        default:
          SPDLOG_WARN("Unknown key sequence element type");
          break;
      }
    }

    //    QThread::sleep(sleep_t);
    const auto kEndTime{std::chrono::high_resolution_clock::now()};
    const auto kDurationMs{std::chrono::duration<double>(kEndTime - kBeforeStart).count() * 1000};
    SPDLOG_INFO("Key sequence executed in {}ms", kDurationMs);
  });
}

void AsynchronousKeySeqThread(const KeySequence& seq) {
  KeySequencerThread::I().SetSequence(seq);
}

KeySequencerThread::~KeySequencerThread() {
  SPDLOG_DEBUG("Destroyed KeySequencerThread");
}

void KeySequencerThread::Run() {
  while (m_continue) {
    Loop();
  }
}

void KeySequencerThread::Stop() {
  I().m_continue = false;
}

void KeySequencerThread::SetSequence(const KeySequence& seq) {
  std::scoped_lock lock{m_mutex};
  m_seq = seq;
  m_current_element = {};
  m_seq_set_time = Now();
  m_last_loop_was_empty = false;

  while (!m_pressed_keys.empty()) {
    const auto kKey{*(m_pressed_keys.begin())};
    win::SendKeyboardEvent(kKey, false);
    m_pressed_keys.erase(kKey);
  }
}

inline void Sleep1Ms() {
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void KeySequencerThread::Loop() {
  if (m_current_element) {
    if (m_current_element->GetKind() != KeySequenceElementKind::Delay) {
      m_current_element = {};
      SPDLOG_WARN("[KEYSEQ] Current element is not a delay element");
      return;
    }

    const auto kDiffFromDelayStartMs{DiffMs(m_delay_start, Now())};
    // SPDLOG_DEBUG("[KEYSEQ] Current delay {}ms", kDiffFromDelayStartMs);
    if (kDiffFromDelayStartMs < m_current_element->GetDelayMs()) {
      const auto kToSkip{static_cast<int>((m_current_element->GetDelayMs() - kDiffFromDelayStartMs) / 3.)};
      if (kToSkip) {
        std::this_thread::sleep_for(std::chrono::milliseconds{kToSkip});
      } else {
        std::this_thread::yield();
      }
      return;
    } else {
      if (kCompleteKeySequencerLogging) SPDLOG_INFO("[KEYSEQ] Delays of {}ms done", m_current_element->GetDelayMs());
      m_current_element = {};
    }
  }

  m_mutex.lock();
  const bool kIsSequenceEmpty{m_seq.Empty()};
  m_mutex.unlock();
  if (!m_last_loop_was_empty && kIsSequenceEmpty) {
    const auto kSeqTime{DiffMs(m_seq_set_time, Now())};
    m_avg_seq_time.AddValue(kSeqTime);
    SPDLOG_INFO("[KEYSEQ] Sequence finished in {}ms", kSeqTime);
    SPDLOG_INFO("[KEYSEQ] Avg {}ms", m_avg_seq_time.GetAverage());
  }
  m_last_loop_was_empty = kIsSequenceEmpty;
  if (kIsSequenceEmpty) {
    Sleep1Ms();
    return;
  }

  m_mutex.lock();
  const auto kCurrentElement{m_seq.Pop()};
  m_mutex.unlock();

  switch (kCurrentElement.GetKind()) {
    case KeySequenceElementKind::Key: {
      if (kCompleteKeySequencerLogging)
        SPDLOG_INFO("[KEYSEQ] Key {} pressed? {}", kCurrentElement.GetKey(), kCurrentElement.GetKeyPressed());
      win::SendKeyboardEvent(kCurrentElement.GetKey(), kCurrentElement.GetKeyPressed());
      if (kCurrentElement.GetKeyPressed()) {
        m_pressed_keys.insert(kCurrentElement.GetKey());
      } else {
        m_pressed_keys.erase(kCurrentElement.GetKey());
      }
    } break;
    case KeySequenceElementKind::Delay: {
      m_current_element = kCurrentElement;
      m_delay_start = Now();
    } break;
    default:
      SPDLOG_WARN("[KEYSEQ] Unknown key sequence element type: {}", static_cast<int>(kCurrentElement.GetKind()));
      break;
  }

  std::this_thread::yield();
}

}  // namespace btc2::io
