#include "controller_handler.hpp"

#include <Logger/logger.hpp>
#include <QSDL/sdl_event_handler.hpp>

namespace btc2 {

ControllerHandler::ControllerHandler()
    : QObject{nullptr}, m_game_controller{std::make_unique<qsdl::GameController>(-1, nullptr)} {
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

  qsdl::SDLEventHandler::Start();
  qsdl::SDLEventHandler::RegisterController(m_game_controller.get());
}

void ControllerHandler::OnControllerPluggedIn(int controller_id) {
  SPDLOG_DEBUG("Controller plugged in: <{}>", controller_id);
}
void ControllerHandler::OnControllerUnplugged(int controller_id) {
  SPDLOG_DEBUG("Controller unplugged: <{}>", controller_id);
}

void ControllerHandler::OnButtonDown(int button) {
  SPDLOG_DEBUG("Button down from <{}>: <{}>", m_game_controller->Id(), button);
}
void ControllerHandler::OnButtonUp(int button) {
  SPDLOG_DEBUG("Button up from <{}>: <{}>", m_game_controller->Id(), button);
}

}  // namespace btc2
