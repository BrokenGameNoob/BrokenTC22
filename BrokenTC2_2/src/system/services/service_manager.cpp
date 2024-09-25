#include "service_manager.hpp"

#include <QDebug>
#include <QtConcurrent>

#include <DataStructures/path_utils.hpp>
#include <DataStructures/structures.hpp>
#include <Logger/logger.hpp>
#include <Utils/json_utils.hpp>
#include <games/gear_handler_factory.hpp>
#include <utils/qt_utils.hpp>
#include <utils/time.hpp>

#include "QSDL/sdl_event_handler.hpp"

namespace btc2 {

ServiceManager::~ServiceManager() {
  SPDLOG_DEBUG("Deleting service manager");
}

void ServiceManager::Init() {
  CREGISTER_QML_SINGLETON_I(btc2, ServiceManager);
}

ServiceManager::ServiceManager()
    : m_settings{std::make_unique<ApplicationSettings>(path::GetApplicationSettingsPath(), nullptr)},
      m_game_profiles_handler{std::make_unique<GameProfilesHandler>()},
      m_controller_handler{std::make_unique<ControllerHandler>()},
      m_keyboard_handler{std::make_unique<KeyboardHandler>()},
      m_game_selector{std::make_unique<GameSelector>()},
      m_gear_handler{} {
  connect(m_game_selector.get(), &GameSelector::gameChanged, this, [this]() {
    m_game_profiles_handler->SetCurrentGame(m_game_selector->GetSelectedGame());
  });

  connect(m_game_selector.get(), &GameSelector::gameChanged, this, [this]() {
    m_gear_handler = MakeGearHandler(m_game_selector->GetSelectedGame());
    emit gearHandlerChanged();
  });

  // emit m_game_selector->gameChanged();
}

void ServiceManager::OnMainWindowLoaded() {}

void ServiceManager::UpdateSDLAxisThreshold(double threshold) {
  m_sdl_axis_threshold = threshold;
  const auto kActualThreshold =
      static_cast<int16_t>(static_cast<double>(std::numeric_limits<int16_t>::max()) * threshold);
  qsdl::SDLEventHandler::SetJoyAxisThreshold(kActualThreshold);
  emit sdlAxisThresholdModified();
}

void ServiceManager::test() {
  SPDLOG_INFO("Test function called");
}

}  // namespace btc2
