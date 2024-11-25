#pragma once

#include <QObject>
#include <QQmlEngine>
#include <git_version.hpp>

#include <DataStructures/structures.hpp>
#include <DataStructures/structures_utils.hpp>
#include <Logger/btype.hpp>
#include <QSDL/game_controller.hpp>
#include <WinUtils/window_change_hook.hpp>
#include <games/gear_handler_factory.hpp>
#include <games/gear_handler_the_crew.hpp>
#include <system/services/controller_handler.hpp>
#include <system/services/game_profiles_handler.hpp>
#include <system/services/keyboard_handler.hpp>
#include <system/services/screen_overlay_selector.hpp>

namespace btc2 {

class ServiceManager : public QObject {
  Q_OBJECT
  QML_SINGLETON

  Q_PROPERTY(bool hasDebInfo READ HasDebInfo CONSTANT FINAL);
  Q_PROPERTY(QString versionStr READ GetVersionStr CONSTANT FINAL);
  Q_PROPERTY(ApplicationSettings* settings READ GetRawSettings CONSTANT FINAL);
  Q_PROPERTY(BaseGearHandler* gearHandler READ GetRawGearHandler NOTIFY gearHandlerChanged FINAL);
  Q_PROPERTY(ControllerHandler* controllerHandler READ GetRawControllerHandler CONSTANT FINAL);
  Q_PROPERTY(KeyboardHandler* keyboardHandler READ GetRawKeyboardHandler CONSTANT FINAL);
  Q_PROPERTY(GameSelector* gameSelector READ GetRawGameSelector CONSTANT FINAL);
  Q_PROPERTY(GameProfilesHandler* gameProfilesHandler READ GetRawGameProfilesHandler CONSTANT FINAL);

  Q_PROPERTY(GameOverlayData* gameOverlay READ GetRawGameOverlay CONSTANT FINAL);
  Q_PROPERTY(QString overlayNotificationText MEMBER m_overlay_notification_text NOTIFY overlayNotificationUpdated)

  Q_PROPERTY(KeyboardProfile* keyboardProfile READ GetRawActiveKeyboardProfile CONSTANT FINAL);
  Q_PROPERTY(bool hasKeyboardConflicts READ AreThereKeyboardConflicts NOTIFY conflictsUpdated FINAL);
  Q_PROPERTY(QStringList keyboardProfileConflicts READ GetKeyboardProfileConflicts NOTIFY conflictsUpdated FINAL);
  Q_PROPERTY(QStringList gameProfileConflicts READ GetGameProfileConflicts NOTIFY conflictsUpdated FINAL);

  Q_PROPERTY(double sdlAxisThreshold READ GetSDLAxisThreshold WRITE UpdateSDLAxisThreshold NOTIFY
                 sdlAxisThresholdModified FINAL)

  Q_PROPERTY(QString focusedWindowTitle READ GetFocusedWindowTitle NOTIFY focusedWindowTitleChanged FINAL)
  Q_PROPERTY(Game::Types focusedGame READ GetFocusedWindowGame NOTIFY focusedWindowTitleChanged FINAL)

 signals:
  void gearHandlerChanged();
  void sdlAxisThresholdModified();
  void focusedWindowTitleChanged();
  void overlayNotificationUpdated();
  void conflictsUpdated();

 public:
  ~ServiceManager();
  static ServiceManager& I() {
    static auto instance{ServiceManager()};
    return instance;
  }

  static void Init();

  /* Test function */
  Q_INVOKABLE void test();  // NOLINT

  /* Version related */
  QString GetVersionStr() const {
    return QString::fromStdString(gitversion::GetVersionRepresentationString());
  }

  /* Is built in debug ? */
  bool HasDebInfo() const {
    return btype::HasDebInfo();
  }

  /* Global application */
  ApplicationSettings* GetRawSettings() {
    return m_settings.get();
  }
  static ApplicationSettings& GetSettings() {
    return *(I().m_settings);
  }

  /* Games related */
  auto& GetGameSelector() {
    return m_game_selector;
  }
  GameSelector* GetRawGameSelector() {
    return m_game_selector.get();
  }

  static BaseGearHandler& GetGearHandler() {
    return *(I().m_gear_handler);
  }
  BaseGearHandler* GetRawGearHandler() {
    return m_gear_handler.get();
  }

  static GameProfilesHandler& GetGameProfilesHandler() {
    return *(I().m_game_profiles_handler);
  }
  GameProfilesHandler* GetRawGameProfilesHandler() {
    return m_game_profiles_handler.get();
  }

  GameOverlayData* GetRawGameOverlay() {
    return m_game_overlay.get();
  }

  Q_INVOKABLE void PublishOverlayNotification(const QString& text, int duration_ms);

  /* Screen related */
  ScreenOverlaySelector* GetRawScreenOverlaySelector() {
    return m_screen_overlay_selector.get();
  }

  Q_INVOKABLE static QStringList GetAvailableScreens() {
    return ScreenOverlaySelector::GetAvailableScreens();
  }

  /* Input related */
  ControllerHandler* GetRawControllerHandler() {
    return m_controller_handler.get();
  }
  KeyboardHandler* GetRawKeyboardHandler() {
    return m_keyboard_handler.get();
  }

  btc2::KeyboardProfile* GetRawActiveKeyboardProfile() {
    return m_keyboard_profile.get();
  }

  Q_INVOKABLE void UpdateSDLAxisThreshold(double threshold);
  double GetSDLAxisThreshold() const {
    return m_sdl_axis_threshold;
  }

  /* Keyboard */
  bool AreThereKeyboardConflicts() const {
    return m_keyboard_conflicts.HasConflicts();
  }

  const QStringList& GetKeyboardProfileConflicts() const {
    return m_keyboard_conflicts.right_conflicts;
  }

  const QStringList& GetGameProfileConflicts() const {
    return m_keyboard_conflicts.left_conflicts;
  }

  /* Windows related */
  static void CALLBACK OnWindowChangeHook(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild,
                                          DWORD dwEventThread, DWORD dwmsEventTime);
  void OnFocusedWindowChanged(const QString& title);
  QString GetFocusedWindowTitle() const {
    return m_focused_window_title;
  }
  Game::Types GetFocusedWindowGame() const;

  /* Main */
  Q_INVOKABLE void OnMainWindowLoaded();

 private:
  ServiceManager();

  void UpdateGearHandlerSoftEnabling(); /* Disabled inputs if key conflicts or not the right game */
  void UpdateKeyboardConflicts();

 private:
  std::unique_ptr<ApplicationSettings> m_settings{nullptr};

  std::unique_ptr<GameProfilesHandler> m_game_profiles_handler{nullptr};

  std::unique_ptr<ControllerHandler> m_controller_handler{nullptr};
  std::unique_ptr<KeyboardHandler> m_keyboard_handler{nullptr};

  std::unique_ptr<GameSelector> m_game_selector{nullptr};
  std::unique_ptr<BaseGearHandler> m_gear_handler{nullptr};
  std::unique_ptr<GameOverlayData> m_game_overlay{nullptr};
  QString m_overlay_notification_text{};
  QTimer m_overlay_notification_timer{};

  std::unique_ptr<ScreenOverlaySelector> m_screen_overlay_selector{nullptr};

  std::shared_ptr<KeyboardProfile> m_keyboard_profile{nullptr};
  ConflictsResults m_keyboard_conflicts;

  win::WinHookOwner m_window_change_hook;
  QString m_focused_window_title;
  Game::Types m_game_focused{Game::NONE};
  Game::Types m_latest_known_game_focused{Game::NONE};

  double m_sdl_axis_threshold{0.5};
};

}  // namespace btc2
