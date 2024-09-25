#pragma once

#include <QObject>
#include <QQmlEngine>
#include <git_version.hpp>

#include <DataStructures/structures.hpp>
#include <Logger/btype.hpp>
#include <QSDL/game_controller.hpp>
#include <games/gear_handler_factory.hpp>
#include <games/gear_handler_the_crew.hpp>
#include <system/services/controller_handler.hpp>
#include <system/services/game_profiles_handler.hpp>
#include <system/services/keyboard_handler.hpp>

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

  Q_PROPERTY(double sdlAxisThreshold READ GetSDLAxisThreshold WRITE UpdateSDLAxisThreshold NOTIFY
                 sdlAxisThresholdModified FINAL)

 signals:
  void gearHandlerChanged();
  void sdlAxisThresholdModified();

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

  /* Input related */
  ControllerHandler* GetRawControllerHandler() {
    return m_controller_handler.get();
  }
  KeyboardHandler* GetRawKeyboardHandler() {
    return m_keyboard_handler.get();
  }

  Q_INVOKABLE void UpdateSDLAxisThreshold(double threshold);
  double GetSDLAxisThreshold() const {
    return m_sdl_axis_threshold;
  }

  /* Main */
  Q_INVOKABLE void OnMainWindowLoaded();

 private:
  ServiceManager();

  std::unique_ptr<ApplicationSettings> m_settings{nullptr};

  std::unique_ptr<GameProfilesHandler> m_game_profiles_handler{nullptr};

  std::unique_ptr<ControllerHandler> m_controller_handler{nullptr};
  std::unique_ptr<KeyboardHandler> m_keyboard_handler{nullptr};

  std::unique_ptr<GameSelector> m_game_selector{nullptr};
  std::unique_ptr<BaseGearHandler> m_gear_handler{nullptr};

  double m_sdl_axis_threshold{0.5};
};

}  // namespace btc2
