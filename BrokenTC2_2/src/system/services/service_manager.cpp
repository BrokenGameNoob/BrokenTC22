#include "service_manager.hpp"

#include <QtConcurrent>

#include <system/logs/logger.hpp>
#include <utils/qt_utils.hpp>
#include <utils/time.hpp>

namespace btc2 {

ServiceManager::~ServiceManager() {
  SPDLOG_DEBUG("Deleting service manager");
}

void ServiceManager::Init() {
  SPDLOG_INFO("Init");

  CREGISTER_QML_SINGLETON_I(btc2, ServiceManager);
}

ServiceManager::ServiceManager() {
  m_gear_handler = std::make_unique<GearHandlerTheCrew>(nullptr);
}

void ServiceManager::test() {
  SPDLOG_DEBUG("Starting concurrent");
  const auto kBeforeStart{NowMs()};
  std::ignore = QtConcurrent::run([&, kBeforeStart]() {
    const auto kStarted{NowMs()};
    PrintTimeDiff("Time before function start", kBeforeStart, kStarted);
    int sleep_t = 1000;
    SPDLOG_DEBUG("Sleeping for: {}ms", sleep_t);
    const auto kStartTime{std::chrono::high_resolution_clock::now()};
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_t));
    //    QThread::sleep(sleep_t);
    const auto kEndTime{std::chrono::high_resolution_clock::now()};
    const auto kDurationMs{std::chrono::duration<double>(kEndTime - kStartTime).count() * 1000};
    SPDLOG_INFO("Has slept for: {}ms", kDurationMs);
  });
}

}  // namespace btc2
