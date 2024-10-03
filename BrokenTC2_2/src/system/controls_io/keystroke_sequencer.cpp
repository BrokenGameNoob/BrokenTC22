#include "keystroke_sequencer.hpp"

#include <QtConcurrent>

#include <Logger/logger.hpp>
#include <WinUtils/event_handler.hpp>
#include <utils/time.hpp>

namespace btc2::io {

void AsynchronousKeySeq(const KeySequence& seq) {
  const auto kBeforeStart{NowMs()};
  std::ignore = QtConcurrent::run([seq, kBeforeStart]() {
    const auto kStarted{NowMs()};
    PrintTimeDiff("Time before function start", kBeforeStart, kStarted);
    const auto kStartTime{std::chrono::high_resolution_clock::now()};

    for (const auto& e : seq) {
      switch (e.GetKind()) {
        case KeySequenceElementKind::Key: {
          auto kBefDelay{NowMs()};

          SPDLOG_INFO("Key {} pressed? {}", e.GetKey(), e.GetKeyPressed());
          win::SendKeyboardEvent(e.GetKey(), e.GetKeyPressed());

          auto kAfterDelay{NowMs()};
          SPDLOG_WARN("Key action time of {}ms", DiffMs(kBefDelay, kAfterDelay));
        } break;
        case KeySequenceElementKind::Delay: {
          auto kBefDelay{NowMs()};
          //          std::this_thread::sleep_for(std::chrono::milliseconds(e.GetDelayMs()));
          QThread::msleep(e.GetDelayMs());
          auto kAfterDelay{NowMs()};
          SPDLOG_TRACE("Delay of {}ms (actual {}ms)", e.GetDelayMs(), DiffMs(kBefDelay, kAfterDelay));
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

KeySequencerThread::~KeySequencerThread(){
  SPDLOG_DEBUG("Destroyed KeySequencerThread");
}

void KeySequencerThread::Run(){
  while(m_continue){
    Loop();
  }
}

void KeySequencerThread::Stop(){
  I().m_continue = false;
}

void KeySequencerThread::Loop(){
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  SPDLOG_DEBUG("HEY");
}

}  // namespace btc2::io
