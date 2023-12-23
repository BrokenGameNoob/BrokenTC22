#include "service_manager.hpp"

#include <QtConcurrent>

#include <Logger/logger.hpp>
#include <ProtoMessages/soft_controls.hpp>
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

ServiceManager::ServiceManager() : m_gear_handler{std::make_unique<GearHandlerTheCrew>(nullptr)} {
  //  m_tmp.actions()[0] = {};
}

void ServiceManager::OnMainWindowLoaded() {
  m_controller_handler = std::make_unique<ControllerHandler>();
}

void ServiceManager::test() {
  ForEach<SoftAction::Available>([&](auto val, const char* key, const QMetaEnum& e) {
    SPDLOG_DEBUG("NAME: {}     Kb?: {}   Controller?: {}", key, IsKeyboardCompatible(val), IsControllerCompatible(val));
    return false;
  });
}

}  // namespace btc2
