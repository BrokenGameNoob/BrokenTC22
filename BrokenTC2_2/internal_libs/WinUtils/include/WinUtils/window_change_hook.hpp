#pragma once

#include <QString>
#include <windows.h>

#include <Logger/logger.hpp>

namespace win {

inline QString GetFocusedWindowTitle() {
  // Get the handle of the currently focused window
  HWND hwnd = GetForegroundWindow();
  if (hwnd == NULL) {
    return QString();
  }

  // Get the length of the window title
  int length = GetWindowTextLength(hwnd);
  if (length == 0) {
    return QString();
  }

  // Retrieve the window title
  std::wstring title(length + 1, L'\0');
  GetWindowText(hwnd, &title[0], length + 1);

  // Convert std::wstring to QString and return
  return QString::fromStdWString(title);
}

class WinHookOwner {
 public:
  WinHookOwner(HWINEVENTHOOK hook) : m_hook{hook} {}
  WinHookOwner() = delete;
  WinHookOwner(const WinHookOwner&) = delete;
  WinHookOwner(WinHookOwner&&) = default;
  WinHookOwner& operator=(const WinHookOwner&) = delete;

  ~WinHookOwner() {
    if (m_hook) {
      UnhookWinEvent(m_hook);
      SPDLOG_DEBUG("Unhooked event hook");
    }
  }

 private:
  HWINEVENTHOOK m_hook;
};

inline WinHookOwner HookForFocusedWindowChanged(WINEVENTPROC callback) {
  return WinHookOwner{SetWinEventHook(EVENT_SYSTEM_FOREGROUND,
                                      EVENT_SYSTEM_FOREGROUND,
                                      NULL,
                                      callback,
                                      0,
                                      0,
                                      WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS)};
}

}  // namespace win
