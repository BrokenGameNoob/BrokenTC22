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

#include <QObject>
#include <QVector>
#include <SDL2/SDL_joystick.h>

namespace qsdl {

//---------------------------------------------------
//*
//*         GameController
//*
//---------------------------------------------------

class GameController : public QObject {
  Q_OBJECT

 signals:
  void unregisterFromEventHandler(qsdl::GameController*);

  void buttonDown(int button);
  void buttonUp(int button);

 public:
  explicit GameController(int controller_id = -1, QObject* parent = nullptr);
  ~GameController();

  int Id() {
    return (m_controller_id == -1) ? -1 : SDL_JoystickInstanceID(m_controller_instance);
  }

 public slots:
  void ConnectController(int controller_id);
  void DisconnectController(bool unregister_event_handler);

  void NotifyButtonDown(int button);
  void NotifyButtonUp(int button);

 private:
  int m_controller_id;
  SDL_Joystick* m_controller_instance;
};

}  // namespace qsdl
