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

namespace btc2 {

class ServiceManager : public QObject {
  Q_OBJECT
  QML_SINGLETON

  Q_PROPERTY(bool hasDebInfo READ HasDebInfo CONSTANT FINAL);
  Q_PROPERTY(QString versionStr READ GetVersionStr CONSTANT FINAL);
  Q_PROPERTY(ApplicationSettings* settings READ GetRawSettings CONSTANT FINAL);
  Q_PROPERTY(BaseGearHandler* gearHandler READ GetRawGearHandler CONSTANT FINAL);
  Q_PROPERTY(ControllerHandler* controllerHandler READ GetRawControllerHandler CONSTANT FINAL);
  Q_PROPERTY(GameSelector* gameSelector READ GetRawGameSelector CONSTANT FINAL);

  Q_PROPERTY(ControllerProfile* controllerProfile READ GetRawActiveControllerProfile CONSTANT FINAL);
  Q_PROPERTY(KeyboardProfile* keyboardProfile READ GetRawActiveKeyboardProfile CONSTANT FINAL);

 signals:
  void gearHandlerChanged();

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

  /* Input related */
  ControllerHandler* GetRawControllerHandler() {
    return m_controller_handler.get();
  }

  btc2::ControllerProfile* GetRawActiveControllerProfile() {
    return m_active_controller_profile.get();
  }

  btc2::KeyboardProfile* GetRawActiveKeyboardProfile() {
    return m_keyboard_profile.get();
  }

  /* Main */
  Q_INVOKABLE void OnMainWindowLoaded();

 private:
  ServiceManager();

  std::unique_ptr<ApplicationSettings> m_settings{nullptr};

  std::unique_ptr<ControllerHandler> m_controller_handler{nullptr};

  std::unique_ptr<GameSelector> m_game_selector{nullptr};
  std::unique_ptr<BaseGearHandler> m_gear_handler{nullptr};

  std::shared_ptr<btc2::ControllerProfile> m_active_controller_profile{};
  std::unique_ptr<KeyboardProfile> m_keyboard_profile{nullptr};
};

}  // namespace btc2
