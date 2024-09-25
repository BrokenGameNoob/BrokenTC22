#include "keystroke_sequencer.hpp"

#include <QtConcurrent>

#include <Logger/logger.hpp>
#include <WinUtils/event_handler.hpp>
#include <utils/time.hpp>

namespace btc2::io {

void AsynchronousKeySeq(const KeySequence& seq) {
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

}  // namespace btc2::io
