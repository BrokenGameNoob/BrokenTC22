#include "service_manager.hpp"

#include <QDebug>
#include <QtConcurrent>
#include <windows.h>

#include <DataStructures/path_utils.hpp>
#include <DataStructures/structures.hpp>
#include <Logger/logger.hpp>
#include <Utils/json_utils.hpp>
#include <games/gear_handler_factory.hpp>
#include <system/services/model_registry.hpp>
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
    : m_settings{std::make_shared<ApplicationSettings>(path::GetApplicationSettingsPath(), nullptr)},
      m_game_profiles_handler{std::make_unique<GameProfilesHandler>()},
      m_controller_handler{std::make_unique<ControllerHandler>()},
      m_keyboard_handler{std::make_unique<KeyboardHandler>()},
      m_game_selector{std::make_unique<GameSelector>()},
      m_gear_handler{new GearHandlerTheCrew{nullptr}},
      m_game_overlay{std::make_unique<GameOverlayData>(path::GetOverlaySettingsPath(), nullptr)},
      m_screen_overlay_selector{std::make_unique<ScreenOverlaySelector>(nullptr)},
      m_keyboard_profile{ModelRegistry::GetKeyboardProfile()},
      m_window_change_hook{win::HookForFocusedWindowChanged(ServiceManager::OnWindowChangeHook)},
      m_text_to_speech_manager{std::make_unique<TextToSpeechManager>(m_settings)} {
  connect(m_game_selector.get(), &GameSelector::gameChanged, this, [this]() {
    m_game_profiles_handler->SetCurrentGame(m_game_selector->GetSelectedGame());
    m_settings->SetSelectedGameName(m_game_selector->GetSelectedGameName());
  });
  connect(this, &ServiceManager::focusedWindowTitleChanged, this, [this]() {
    const auto kFocusedGame{GetFocusedWindowGame()};
    m_game_selector->OnFocusedWindowChanged(GetFocusedWindowGame());
  });

  connect(m_game_selector.get(), &GameSelector::gameChanged, this, [this]() {
    m_gear_handler = MakeGearHandler(m_game_selector->GetSelectedGame());
    const auto kNotifString{QObject::tr("Game changed to: %0").arg(m_game_selector->GetSelectedGameName())};
    PublishOverlayNotification(kNotifString, 2000);
    m_text_to_speech_manager->Speak(kNotifString);
    emit gearHandlerChanged();
  });

  connect(&m_overlay_notification_timer, &QTimer::timeout, this, [this]() {
    m_overlay_notification_text.clear();
    emit overlayNotificationUpdated();
  });

  connect(m_screen_overlay_selector.get(), &ScreenOverlaySelector::selectedScreenChanged, this, [this]() {
    m_settings->SetSelectedOverlayScreen(m_screen_overlay_selector->GetSelectedScreenName());
  });
  connect(m_settings.get(), &ApplicationSettings::dataChanged, this, [this]() {
    m_screen_overlay_selector->SetSelectedScreenName(m_settings->SelectedOverlayScreen());
    m_text_to_speech_manager->SetEnabled(m_settings->TTSEnabled());
  });

  connect(m_game_profiles_handler.get(), &GameProfilesHandler::currentGameChanged, this, [this]() {
    UpdateKeyboardConflicts();
  });
  connect(m_game_profiles_handler.get(), &GameProfilesHandler::profileUpdated, this, [this]() {
    UpdateKeyboardConflicts();
  });
  connect(m_keyboard_profile.get(), &KeyboardProfile::dataChanged, this, [this]() { UpdateKeyboardConflicts(); });
  connect(this, &ServiceManager::conflictsUpdated, this, [this]() { UpdateGearHandlerSoftEnabling(); });
  connect(this, &ServiceManager::focusedWindowTitleChanged, this, [this]() { UpdateGearHandlerSoftEnabling(); });
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
  m_focused_window_title = title;

  const auto kFocusedGame{GetFocusedGameFromWindowTitle(m_focused_window_title)};
  m_game_focused = kFocusedGame;
  if (m_game_focused != Game::Types::NONE) {
    m_latest_known_game_focused = m_game_focused;
  } else if (title.contains("Form1") ||
             title.contains("The Crew 2 - Competizione")) { /* Window title of Competizione HUD */
    /*If Competizione HUD took foreground instead of the game, retrieve the last known game.*/
    m_game_focused = m_latest_known_game_focused;
  }

  emit focusedWindowTitleChanged();
}

Game::Types ServiceManager::GetFocusedWindowGame() const {
  return m_game_focused;
}

void ServiceManager::OnMainWindowLoaded() {
  m_focused_window_title = win::GetFocusedWindowTitle();
  OnFocusedWindowChanged(m_focused_window_title);
  UpdateKeyboardConflicts();
}

void ServiceManager::PublishOverlayNotification(const QString& text, int duration_ms) {
  m_overlay_notification_text = text;
  m_overlay_notification_timer.stop();
  m_overlay_notification_timer.setInterval(duration_ms);
  m_overlay_notification_timer.start();
  emit overlayNotificationUpdated();
}

void ServiceManager::UpdateSDLAxisThreshold(double threshold) {
  m_sdl_axis_threshold = threshold;
  const auto kActualThreshold =
      static_cast<int16_t>(static_cast<double>(std::numeric_limits<int16_t>::max()) * threshold);
  qsdl::SDLEventHandler::SetJoyAxisThreshold(kActualThreshold);
  emit sdlAxisThresholdModified();
}

void ServiceManager::test() {
  SPDLOG_INFO("Test function called");
  m_text_to_speech_manager->Speak("Hello world!");
}

void ServiceManager::UpdateGearHandlerSoftEnabling() {
  const bool kRightGameFocused{m_game_focused != Game::Types::NONE};
  GetGearHandler().SetSoftEnabled(kRightGameFocused && !AreThereKeyboardConflicts());
}

void ServiceManager::UpdateKeyboardConflicts() {
  m_keyboard_conflicts = m_game_profiles_handler->UpdateConflicts(*m_keyboard_profile);
  emit conflictsUpdated();
}

}  // namespace btc2
