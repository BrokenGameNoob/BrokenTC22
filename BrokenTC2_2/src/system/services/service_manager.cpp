#include "service_manager.hpp"

#include <QDebug>
#include <QtConcurrent>
#include <windows.h>

#include <DataStructures/path_utils.hpp>
#include <DataStructures/structures.hpp>
#include <Logger/logger.hpp>
#include <Utils/json_utils.hpp>
#include <utils/qt_utils.hpp>
#include <utils/time.hpp>

#include "QSDL/sdl_event_handler.hpp"
#include "system/controls_io/keystroke_sequencer.hpp"

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
      m_game_overlay{std::make_unique<GameOverlay>(path::GetOverlaySettingsPath(), nullptr)},
      m_keyboard_profile{std::make_unique<KeyboardProfile>(path::GetKeyboardProfilePath(), nullptr)},
      m_window_change_hook{win::HookForFocusedWindowChanged(ServiceManager::OnWindowChangeHook)} {
  //  m_tmp.actions()[0] = {};
}

void CALLBACK ServiceManager::OnWindowChangeHook(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject,
                                                 LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime) {
  if (event == EVENT_SYSTEM_FOREGROUND) {
    char windowTitle[256];
    GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));
    std::string title(windowTitle);
    ServiceManager::I().OnFocusedWindowChanged(QString{windowTitle});
  }
}
void ServiceManager::OnFocusedWindowChanged(const QString& title) {
  SPDLOG_DEBUG("Focused window changed to: <{}>", title);
  m_focused_window_title = title;
  emit focusedWindowTitleChanged();
}

Game::Types ServiceManager::GetFocusedWindowGame() const {
  return GetFocusedGameFromWindowTitle(m_focused_window_title);
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
  io::KeySequence ks{
      {1000}, {VK_NUMPAD1, true}, {50}, {VK_NUMPAD1, false}, {500}, {VK_NUMPAD3, true}, {50}, {VK_NUMPAD3, false}};
  io::AsynchronousKeySeqThread(ks);
}

}  // namespace btc2
