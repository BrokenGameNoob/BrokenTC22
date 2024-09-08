#include "service_manager.hpp"

#include <QDebug>
#include <QtConcurrent>

#include <DataStructures/path_utils.hpp>
#include <DataStructures/structures.hpp>
#include <Logger/logger.hpp>
#include <Utils/json_utils.hpp>
#include <utils/qt_utils.hpp>
#include <utils/time.hpp>

namespace btc2 {

ServiceManager::~ServiceManager() {
  SPDLOG_DEBUG("Deleting service manager");
}

void ServiceManager::Init() {
  CREGISTER_QML_SINGLETON_I(btc2, ServiceManager);
}

ServiceManager::ServiceManager()
    : m_settings{std::make_unique<ApplicationSettings>(path::GetApplicationSettingsPath(), nullptr)},
      m_controller_handler{std::make_unique<ControllerHandler>()},
      m_game_selector{std::make_unique<GameSelector>()},
      m_gear_handler{std::make_unique<GearHandlerTheCrew>(nullptr)},
      m_active_controller_profile{std::make_unique<ControllerProfile>(nullptr)},
      m_keyboard_profile{std::make_unique<KeyboardProfile>(path::GetKeyboardProfilePath(), nullptr)} {
  //  m_tmp.actions()[0] = {};

  m_active_controller_profile->SetSavePath("HEY.json", true);
}

void ServiceManager::OnMainWindowLoaded() {}

void ServiceManager::test() {
  SPDLOG_INFO("Test function called");
  SPDLOG_INFO("{}", *m_active_controller_profile);
}

}  // namespace btc2
