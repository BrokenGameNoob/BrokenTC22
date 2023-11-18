/* Broken The Crew 2 sequential clutch assist
 * Copyright (C) 2022 BrokenGameNoob <brokengamenoob@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QObject>
#include <QVector>

#include <Logger/logger.hpp>
#include <QSDL/game_controller.hpp>
#include <QSDL/sdl_global.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_joystick.h>

namespace qsdl {

GameController::GameController(int controller_id, QObject *parent)
    : QObject{parent}, m_controller_id{controller_id}, m_controller_instance{nullptr} {
  if (controller_id >= 0) {
    ConnectController(controller_id);
  }
}

GameController::~GameController() {
  DisconnectController(true);
}

void GameController::ConnectController(int controller_id) {
  if (m_controller_id >= 0)  // if the controller is already connected
  {
    DisconnectController(false);  // disconnect it
  }

  auto joystickCount{getPluggedJoysticksCount()};
  if (controller_id >= joystickCount) {
    m_controller_id = -1;
    SPDLOG_ERROR("Invalid controller ID ({}), there is only {} devices plugged in:\n{}",
                 controller_id,
                 getPluggedJoysticksCount(),
                 SDL_GetError());
  } else {
    m_controller_id = controller_id;
    m_controller_instance = SDL_JoystickOpen(m_controller_id);
    if (!m_controller_instance) {
      SPDLOG_ERROR("Unable to open game controller device (id=%0):\n{}", m_controller_id, SDL_GetError());
    }
  }
}

void GameController::DisconnectController(bool unregister_event_handler) {
  if (m_controller_instance) {
    SDL_JoystickClose(m_controller_instance);
    m_controller_instance = nullptr;
    m_controller_id = -1;
  }
  if (unregister_event_handler) emit unregisterFromEventHandler(this);
}

void GameController::NotifyButtonDown(int button) {
  emit buttonDown(button);
}

void GameController::NotifyButtonUp(int button) {
  emit buttonUp(button);
}

}  // namespace qsdl
