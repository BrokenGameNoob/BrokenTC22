#pragma once

#include <QString>
#include <windows.h>

#include <Logger/logger.hpp>

namespace win {

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
