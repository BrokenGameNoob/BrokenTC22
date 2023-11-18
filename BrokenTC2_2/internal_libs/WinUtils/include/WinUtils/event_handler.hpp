/* reuse of antimicro Gamepad to KB+M event mapper https://github.com/AntiMicroX/antimicrox
 * Copyright (C) 2015 Travis Nickles <nickles.travis@gmail.com>
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

#include <QThread>
#include <windows.h>

#include <WinUtils/utils.hpp>

namespace win {

class WindowsEventSingleton : public QObject {
  Q_OBJECT

 signals:
  void keyDown(int key);

 public:
  static WindowsEventSingleton* Instance() {
    static WindowsEventSingleton m_instance{};
    return &m_instance;
  }
  static WindowsEventSingleton* I() {
    return Instance();
  }

  void IEmitKeyDown(int key) {
    emit keyDown(key);
  }

  static void EmitKeyDown(int key) {
    I()->IEmitKeyDown(key);
  }

  static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

 private:
  WindowsEventSingleton();
  ~WindowsEventSingleton();
  WindowsEventSingleton(const WindowsEventSingleton&) = delete;
  WindowsEventSingleton& operator=(const WindowsEventSingleton&) = delete;
};

//--------------------------------------------------------------

using uint = unsigned int;

inline INPUT GetKbInput(int vk_code, bool pressed) {
  // https://learn.microsoft.com/fr-fr/windows/win32/api/winuser/nf-winuser-sendinput
  INPUT out{};
  out.type = INPUT_KEYBOARD;
  // out.ki.wVk = vk_code;
  //  out.ki.wScan = 0;  // ScancodeFromVirtualKey(vk_code);
  out.ki.wVk = 0;
  out.ki.wScan = ScancodeFromVirtualKey(vk_code);
  out.ki.time = 0;
  out.ki.dwFlags = KEYEVENTF_SCANCODE;

  if (!pressed) {
    out.ki.dwFlags = out.ki.dwFlags | KEYEVENTF_KEYUP;
  }
  return out;
}
void SendKeyboardEvent(int code, bool pressed);

}  // namespace win
