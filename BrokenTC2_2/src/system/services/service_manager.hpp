#pragma once

#include <QObject>
#include <QQmlEngine>
#include <git_version.hpp>

#include <DataStructures/structures.hpp>
#include <Logger/btype.hpp>
#include <QSDL/game_controller.hpp>
#include <WinUtils/window_change_hook.hpp>
#include <games/gear_handler_factory.hpp>
#include <games/gear_handler_the_crew.hpp>
#include <system/services/controller_handler.hpp>

namespace btc2 {

class ServiceManager : public QObject {
  Q_OBJECT
  QML_SINGLETON

  Q_PROPERTY(bool hasDebInfo READ HasDebInfo CONSTANT FINAL);
  Q_PROPERTY(QString versionStr READ GetVersionStr CONSTANT FINAL);
  Q_PROPERTY(ApplicationSettings* settings READ GetRawSettings CONSTANT FINAL);
  Q_PROPERTY(BaseGearHandler* gearHandler READ GetRawGearHandler CONSTANT FINAL);
  Q_PROPERTY(GameOverlay* gameOverlay READ GetRawGameOverlay CONSTANT FINAL);
  Q_PROPERTY(ControllerHandler* controllerHandler READ GetRawControllerHandler CONSTANT FINAL);
  Q_PROPERTY(GameSelector* gameSelector READ GetRawGameSelector CONSTANT FINAL);

  Q_PROPERTY(KeyboardProfile* keyboardProfile READ GetRawActiveKeyboardProfile CONSTANT FINAL);

  Q_PROPERTY(double sdlAxisThreshold READ GetSDLAxisThreshold WRITE UpdateSDLAxisThreshold NOTIFY
                 sdlAxisThresholdModified FINAL)

  Q_PROPERTY(QString focusedWindowTitle READ GetFocusedWindowTitle NOTIFY focusedWindowTitleChanged FINAL)
  Q_PROPERTY(Game::Types focusedGame READ GetFocusedWindowGame NOTIFY focusedWindowTitleChanged FINAL)

 signals:
  void gearHandlerChanged();
  void sdlAxisThresholdModified();
  void focusedWindowTitleChanged();

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

  /* Games related */
  auto& GetGameSelector() {
    return m_game_selector;
  }
  GameSelector* GetRawGameSelector() {
    return m_game_selector.get();
  }

  BaseGearHandler* GetRawGearHandler() {
    return m_gear_handler.get();
  }

  GameOverlay* GetRawGameOverlay() {
    return m_game_overlay.get();
  }

  /* Input related */
  ControllerHandler* GetRawControllerHandler() {
    return m_controller_handler.get();
  }

  btc2::KeyboardProfile* GetRawActiveKeyboardProfile() {
    return m_keyboard_profile.get();
  }

  Q_INVOKABLE void UpdateSDLAxisThreshold(double threshold);
  double GetSDLAxisThreshold() const {
    return m_sdl_axis_threshold;
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

  std::unique_ptr<ApplicationSettings> m_settings{nullptr};

  std::unique_ptr<ControllerHandler> m_controller_handler{nullptr};

  std::unique_ptr<GameSelector> m_game_selector{nullptr};
  std::unique_ptr<BaseGearHandler> m_gear_handler{nullptr};
  std::unique_ptr<GameOverlay> m_game_overlay{nullptr};

  std::unique_ptr<KeyboardProfile> m_keyboard_profile{nullptr};

  win::WinHookOwner m_window_change_hook;
  QString m_focused_window_title;

  double m_sdl_axis_threshold{0.5};
};

}  // namespace btc2
