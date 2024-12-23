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

#include <WinUtils/event_handler.hpp>

/*!
 *  This file contains elements of antimicroX project
 *
 */

#include <QTimer>

#include <Logger/logger.hpp>
#include <WinUtils/utils.hpp>

namespace win {

LRESULT CALLBACK WindowsEventSingleton::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  BOOL fEatKeystroke = FALSE;

  if (nCode == HC_ACTION) {
    PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
    auto vkCode = p->vkCode;
    switch (wParam) {
      case WM_KEYDOWN:
        EmitKeyDown(vkCode);
        break;
      case WM_SYSKEYDOWN:
        break;
      case WM_KEYUP:
        EmitKeyUp(vkCode);
        break;
      case WM_SYSKEYUP:
        break;
      default:
        break;
    }
  }
  return (fEatKeystroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
}

WindowsEventSingleton::WindowsEventSingleton() : QObject() {
  HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, WindowsEventSingleton::LowLevelKeyboardProc, 0, 0);
  std::ignore = hhkLowLevelKybd;
}

WindowsEventSingleton::~WindowsEventSingleton() {}

void SendKeyboardEvent(int code, bool pressed) {
  auto input{GetKbInput(code, pressed)};
  WindowsEventSingleton::SetInhibitEvents(true);
  if (!SendInput(1, &input, sizeof(INPUT))) {
    SPDLOG_WARN("Could not simulate input with vk_code: {} ({})", code, win::VkCodeToStr(code));
  }
  WindowsEventSingleton::SetInhibitEvents(false);
}

}  // namespace win
