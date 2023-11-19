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

#include <chrono>
#include <future>
#include <iostream>
#include <unordered_map>
#include <windows.h>

#include <Logger/logger.hpp>
#include <QSDL/game_controller.hpp>
#include <QSDL/sdl_event_handler.hpp>
#include <QSDL/sdl_global.hpp>
#include <SDL2/SDL.h>

namespace qsdl {

//-----------------------------------------------------------------------

void SDLEventThread::Work(std::shared_ptr<const EventHandlerSharedConfig> shared_config) {
  static SDL_Event e;
  m_continue = true;

  auto deviceId{[&]() { return e.jdevice.which; }};
  static std::unordered_map<int, bool> dpadDown{};  //<Button ID,isDown>
  // Button ID : (button+OFFSET)*10+
  // isDown : true/false

  static std::unordered_map<int, bool> joyMove{};  //<Button ID,isDown>
  // Button ID : (axis+OFFSET)*100+(value<0?1:2)
  // isDown : true/false

  auto lambdaSleep{[&](int ms) { QThread::msleep(ms); }};

  while (m_continue) {
    m_continue = !(shared_config.get()->m_shouldStop);
    if (m_continue == false) {
      SPDLOG_DEBUG("WE SHOULD STOP NOW");
    }

    bool skipToNextIter{false};
    if (shared_config.get()->lowPerfMode()) {
      skipToNextIter = (SDL_PollEvent(&e) == 0);
      if (skipToNextIter) {
        lambdaSleep(15);
      }
    } else {
      skipToNextIter = (SDL_WaitEventTimeout(&e, 100) == 0);
    }

    //    SPDLOG_DEBUG("lowPerfMode={}", shared_config.get()->lowPerfMode());
    //    SPDLOG_DEBUG("skipToNextIter={}", skipToNextIter);
    //    SPDLOG_DEBUG("e.type={}", e.type);

    if (skipToNextIter) continue;

    switch (e.type) {
      case SDL_JOYAXISMOTION: {
        constexpr auto AXIS_BUTTON_OFFSET{1000};
        auto curPos{abs(e.jaxis.value) > shared_config->joyAxisthreshold()};
        auto axis{e.jaxis.axis};
        auto dictKey{(AXIS_BUTTON_OFFSET + axis) * 1000 + (e.jaxis.value < 0 ? 1 : 2) * 100 + deviceId()};

        if (!joyMove.contains(dictKey)) joyMove[dictKey] = curPos;

        if (joyMove.at(dictKey) == false && curPos) {
          emit buttonDown(deviceId(), AXIS_BUTTON_OFFSET + axis * 10 + (e.jaxis.value < 0 ? 1 : 2));
          joyMove[dictKey] = true;
          lambdaSleep(15);
        } else if (joyMove.at(dictKey) == true && !curPos) {
          emit buttonUp(deviceId(), AXIS_BUTTON_OFFSET + axis * 10 + (e.jaxis.value < 0 ? 1 : 2));
          joyMove[dictKey] = false;
          lambdaSleep(15);
        }
        break;
      }
      case SDL_JOYBUTTONDOWN:
        emit buttonDown(deviceId(), e.jbutton.button);
        break;
      case SDL_JOYBUTTONUP:
        emit buttonUp(deviceId(), e.jbutton.button);
        break;
      case SDL_JOYHATMOTION: {
        constexpr auto HAT_BUTTON_OFFSET{100};
        auto curPos{e.jhat.value};

        auto checkPos{[&](const auto& hatVal) {
          auto dictKey{(HAT_BUTTON_OFFSET + hatVal) * 100 + deviceId()};
          if (curPos & hatVal) {
            emit buttonDown(deviceId(), HAT_BUTTON_OFFSET + hatVal);
            dpadDown[dictKey] = true;
          } else if (dpadDown[dictKey] == true) {
            emit buttonUp(deviceId(), HAT_BUTTON_OFFSET + hatVal);
            dpadDown[dictKey] = false;
          }
        }};
        checkPos(SDL_HAT_UP);
        checkPos(SDL_HAT_RIGHT);
        checkPos(SDL_HAT_DOWN);
        checkPos(SDL_HAT_LEFT);
        break;
      }

      case SDL_JOYDEVICEADDED:
        SPDLOG_INFO("New controller plugged in: id={}", e.jdevice.which);
        emit newControllerPluggedIn(e.jdevice.which);
        break;
      case SDL_JOYDEVICEREMOVED:
        SPDLOG_INFO("Controller unplugged: id={}", e.jdevice.which);
        emit controllerUnplugged(e.jdevice.which);
      default:
        break;
    }
  }
}

SDLEventHandler::SDLEventHandler() : QObject(), m_controller_list() {
  if (!InitSDL(SDL_INIT_JOYSTICK)) {
    SPDLOG_ERROR("Cannot initialize SDL2_Joystick:\n{}", SDL_GetError());
    throw std::runtime_error("Cannot initialize SDL2_Joystick");
  }

  auto worker{new SDLEventThread};
  worker->moveToThread(&workerThread);
  connect(this, &SDLEventHandler::operate, worker, &SDLEventThread::Work);

  connect(worker, &SDLEventThread::testSignal, [&]() { qDebug() << "J'ai eu un test !"; });

  connect(worker, &SDLEventThread::newControllerPluggedIn, this, [&](int id) { emit gameControllerAdded(id); });
  connect(worker, &SDLEventThread::controllerUnplugged, this, [&](int id) { emit gameControllerRemoved(id); });

  connect(worker, &SDLEventThread::buttonDown, this, &SDLEventHandler::OnButtonDown);
  connect(worker, &SDLEventThread::buttonUp, this, &SDLEventHandler::OnButtonUp);

  workerThread.start();
}

SDLEventHandler::~SDLEventHandler() {
  m_shared_config.get()->m_shouldStop = true;  // nicely exit event loop
  workerThread.quit();
  workerThread.wait();
}

//***********************************************************
//---------------------------------------------- Event slot
//***********************************************************

void SDLEventHandler::OnButtonDown(int controller_id, int button) {
  for (const auto& e : m_controller_list) {
    if (e->Id() == controller_id) {
      e->NotifyButtonDown(button);
    }
  }
  emit buttonDown(controller_id, controller_id);
}

void SDLEventHandler::OnButtonUp(int controller_id, int button) {
  for (const auto& e : m_controller_list) {
    if (e->Id() == controller_id) {
      e->NotifyButtonUp(button);
    }
  }
  emit buttonUp(controller_id, button);
}

//***********************************************************
//---------------------------------------------- Event slot - END
//***********************************************************

void SDLEventHandler::RegisterController(GameController* g) {
  SPDLOG_INFO("Registering controller: id={}", g->Id());
  connect(g, &GameController::unregisterFromEventHandler, Instance(), &SDLEventHandler::UnregisterController);

  if (Instance()->m_controller_list.indexOf(g) == -1) Instance()->m_controller_list.append(g);
}

void SDLEventHandler::UnregisterController(GameController* g) {
  SPDLOG_INFO("Unregistering controller: id={}", g->Id());
  auto gIndex{Instance()->m_controller_list.indexOf(g)};
  if (gIndex != -1)
    Instance()->m_controller_list.removeAt(gIndex);
  else {
    qWarning() << "Willing to unregister not known controller with id:" << g->Id();
  }
}

}  // namespace qsdl
