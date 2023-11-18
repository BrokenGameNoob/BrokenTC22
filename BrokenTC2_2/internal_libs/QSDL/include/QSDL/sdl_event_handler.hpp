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

#pragma once

#include <QDebug>
#include <QThread>
#include <QVector>
#include <atomic>
#include <chrono>
#include <unordered_map>

#include <QSDL/game_controller.hpp>
#include <QSDL/sdl_global.hpp>

namespace qsdl {

class GameController;

class SDLEventThread : public QObject {
  Q_OBJECT

 public slots:
  void Work(std::shared_ptr<const EventHandlerSharedConfig> shared_config);

 signals:
  void testSignal();
  void buttonDown(int controller_id, int button);
  void buttonUp(int controller_id, int button);

  void newControllerPluggedIn(int controller_id);
  void controllerUnplugged(int controller_id);

 private:
  std::atomic_bool m_continue{true};
};

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------

// Singleton

class SDLEventHandler : public QObject {
  Q_OBJECT

  QThread workerThread;

 public:
  static SDLEventHandler* Instance() {
    static SDLEventHandler m_instance{};
    return &m_instance;
  }

  static void Start() {
    emit Instance()->operate(Instance()->m_shared_config);
  }

  static void SetLowPerfMode(bool enable) {
    Instance()->m_shared_config.get()->setLowPerfMode(enable);
  }

  static void SetJoyAxisThreshold(int16_t threshold) {
    Instance()->m_shared_config.get()->setJoyAxisthreshold(threshold);
  }

 signals:
  void operate(std::shared_ptr<EventHandlerSharedConfig>);

  void gameControllerAdded(int id);
  void gameControllerRemoved(int id);
  void testSignal();

  void buttonDown(int controller_id, int button);
  void buttonUp(int controller_id, int button);

 private slots:
  void OnButtonDown(int controller_id, int button);
  void OnButtonUp(int controller_id, int button);

 public slots:
  static void RegisterController(qsdl::GameController* g);
  static void UnregisterController(qsdl::GameController* g);

 private:
  SDLEventHandler();
  ~SDLEventHandler();
  SDLEventHandler(const SDLEventHandler&) = delete;
  SDLEventHandler& operator=(const SDLEventHandler&) = delete;

  std::shared_ptr<EventHandlerSharedConfig> m_shared_config{
      std::make_shared<EventHandlerSharedConfig>(EventHandlerSharedConfig{false})};

  QVector<GameController*> m_controller_list;
};

}  // namespace qsdl
