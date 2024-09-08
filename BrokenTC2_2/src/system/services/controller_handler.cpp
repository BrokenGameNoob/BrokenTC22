#include "controller_handler.hpp"

#include <Logger/logger.hpp>
#include <QSDL/sdl_event_handler.hpp>
#include <system/services/model_registry.hpp>
#include <system/services/service_manager.hpp>
#include <utils/qt_utils.hpp>

namespace btc2 {

ControllerHandler::ControllerHandler()
    : QObject{nullptr},
      m_game_controller{std::make_unique<qsdl::GameController>(-1, nullptr)},
      m_active_profile{ModelRegistry::GetControllerProfile(tr("Default"))} {
  using qsdl::GameController;
  using qsdl::SDLEventHandler;
  connect(SDLEventHandler::Instance(),
          &SDLEventHandler::gameControllerAdded,
          this,
          &ControllerHandler::OnControllerPluggedIn);
  connect(SDLEventHandler::Instance(),
          &SDLEventHandler::gameControllerRemoved,
          this,
          &ControllerHandler::OnControllerUnplugged);

  connect(m_game_controller.get(), &GameController::buttonDown, this, &ControllerHandler::OnButtonDown);
  connect(m_game_controller.get(), &GameController::buttonUp, this, &ControllerHandler::OnButtonUp);

  qsdl::SDLEventHandler::Start();
  qsdl::SDLEventHandler::RegisterController(m_game_controller.get());
}

void ControllerHandler::Init() {
  CREGISTER_QML_UNCREATABLE_TYPE(btc2, ControllerHandler, "CppOwned");
}

QStringList ControllerHandler::GetControllerList() const {
  return qsdl::GetPluggedJoysticks();
}

ControllerProfile* ControllerHandler::QMLActiveController() const {
  return m_active_profile.get();
}

void ControllerHandler::SetActiveController(const QString& controller_name) {
  if (m_active_profile && controller_name == m_active_profile->Name()) {
    return;
  }
  m_old_profile = m_active_profile;
  m_active_profile = ModelRegistry::GetControllerProfile(controller_name);

  emit activeControllerChanged();
}

void ControllerHandler::OnControllerPluggedIn(int controller_id) {
  SPDLOG_DEBUG("Controller plugged in: <{}>", controller_id);
  m_game_controller->ConnectController(controller_id);

  emit controllerPluggedInOrOut();
}
void ControllerHandler::OnControllerUnplugged(int controller_id) {
  SPDLOG_DEBUG("Controller unplugged: <{}>", controller_id);
  m_game_controller->DisconnectController(false);
  emit controllerPluggedInOrOut();
}

void ControllerHandler::OnButtonDown(int button) {
  SPDLOG_DEBUG("Button down from <{}>: <{}>", m_game_controller->Id(), button);
}
void ControllerHandler::OnButtonUp(int button) {
  SPDLOG_DEBUG("Button up from <{}>: <{}>", m_game_controller->Id(), button);
}

}  // namespace btc2
