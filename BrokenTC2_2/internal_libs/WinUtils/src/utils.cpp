#include <windows.h>

//

#include <QObject>
#include <bitset>
#include <filesystem>
#include <optional>
#include <psapi.h>
#include <tchar.h>

#include <Logger/logger.hpp>
#include <WinUtils/utils.hpp>

namespace {

std::string to_string(wchar_t const* wcstr) {
  auto s = std::mbstate_t();
  auto const target_char_count = std::wcsrtombs(nullptr, &wcstr, 0, &s);
  if (target_char_count == static_cast<std::size_t>(-1)) {
    throw std::logic_error("Illegal byte sequence");
  }

  // +1 because std::string adds a null terminator which isn't part of size
  auto str = std::string(target_char_count, '\0');
  std::wcsrtombs(str.data(), &wcstr, str.size() + 1, &s);
  return str;
}

template <typename T>
std::bitset<sizeof(T) * 8> tob(const T& v) {
  std::bitset<sizeof(T) * 8> rVal{v};
  return rVal;
}

template <typename T>
QString tobs(const T& v) {
  auto bin{tob(v)};
  return QString::fromStdString(bin.to_string());
}

}  // namespace

namespace win {

std::string GetProcessName(DWORD processID) {
  HANDLE Handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                              FALSE,
                              processID /* This is the PID, you can find one from windows task manager */
  );
  if (Handle) {
    TCHAR Buffer[MAX_PATH];
    auto success{GetModuleFileNameEx(Handle, 0, Buffer, MAX_PATH)};
    CloseHandle(Handle);
    if (success) {
      namespace fs = std::filesystem;
      // At this point, buffer contains the full path to the executable
      std::string tmp{to_string(Buffer)};
      tmp = fs::path(tmp).filename().string();
      return tmp;
    } else {
      // You better call GetLastError() here
    }
  }
  return {};
}

bool TerminateProcess(DWORD processID) {
  HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE, FALSE, processID);
  bool success = ::TerminateProcess(hProcess, 1);
  CloseHandle(hProcess);
  return success;
}

bool TerminateProcess(const QString& p_name) {
  const auto kPId{FindProcessId(p_name)};
  if (kPId) {
    const auto kRVal{TerminateProcess(kPId.value())};
    if (!kRVal) {
      SPDLOG_WARN("Failed to terminate process: {}", p_name);
    }
  } else {
    SPDLOG_TRACE("Process id of <{}> not found", p_name);
  }
  return true;
}

std::optional<DWORD> FindProcessId(const QString& pName) {
  DWORD aProcesses[1024], cbNeeded, cProcesses;
  unsigned int i;

  if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
    return {};
  }

  // Calculate how many process identifiers were returned.
  cProcesses = cbNeeded / sizeof(DWORD);

  // Print the name and process identifier for each process.
  for (i = 0; i < cProcesses; i++) {
    if (aProcesses[i] != 0) {
      auto id{aProcesses[i]};

      if (QString::fromStdString(GetProcessName(id)) == pName) {
        return {id};
      }
    }
  }
  return {};
}

std::vector<DWORD> FindProcessesId(const QString& pName) {
  std::vector<DWORD> out{};
  out.reserve(100);

  DWORD aProcesses[1024], cbNeeded, cProcesses;
  unsigned int i;

  if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
    return {};
  }

  // Calculate how many process identifiers were returned.
  cProcesses = cbNeeded / sizeof(DWORD);

  // Print the name and process identifier for each process.
  for (i = 0; i < cProcesses; i++) {
    auto id{aProcesses[i]};
    if (id != 0) {
      if (QString::fromStdString(GetProcessName(id)) == pName) {
        out.emplace_back(aProcesses[i]);
      }
    }
  }
  out.shrink_to_fit();
  return out;
}

std::vector<DWORD> GetAllProcesses() {
  std::vector<DWORD> out{};
  out.reserve(300);

  DWORD aProcesses[1024], cbNeeded, cProcesses;
  unsigned int i;

  if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
    return {};
  }

  // Calculate how many process identifiers were returned.
  cProcesses = cbNeeded / sizeof(DWORD);

  // Print the name and process identifier for each process.
  for (i = 0; i < cProcesses; i++) {
    auto id{aProcesses[i]};
    if (id != 0) {
      out.emplace_back(id);
    }
  }
  out.shrink_to_fit();
  return out;
}

int32_t ProcessCount(const QString& pName) {
  return static_cast<int32_t>(FindProcessesId(pName).size());
}

int32_t GetCoreCount() {
  SYSTEM_INFO SystemInfo;
  GetSystemInfo(&SystemInfo);
  return static_cast<int32_t>(SystemInfo.dwNumberOfProcessors);
}

bool SetCoreCountAffinity(int32_t coreCountToUse, bool throwOnFail, bool verbose) {
  auto lambdaFail{[&](const QString& err) -> bool {
    if (throwOnFail) {
      throw std::runtime_error(err.toStdString());
    }
    return false;
  }};

  if (coreCountToUse > GetCoreCount() || coreCountToUse < 0) {
    QString err{QString{"%0 : Invalid coreCountToUse=%1, its value must be 0 < coreCountToUse > %2"}
                    .arg(__PRETTY_FUNCTION__)
                    .arg(coreCountToUse)
                    .arg(GetCoreCount())};
    return lambdaFail(err);
  }

  HANDLE hProcess = GetCurrentProcess();
  ULONG_PTR appAff;
  ULONG_PTR sysAff;
  if (!GetProcessAffinityMask(hProcess, &appAff, &sysAff))
    return lambdaFail(QString{"%0 : Can't get processor core affinity"}.arg(__PRETTY_FUNCTION__));

  ULONG_PTR aff{};
  for (int32_t i{}; i < coreCountToUse; ++i) {
    aff = (aff << 1u) | 1u;
    //        qDebug() << "   " << aff;
  }
  ULONG_PTR curAff = aff & appAff;

  //    qDebug() << tobs(appAff) << "  " << appAff;
  //    qDebug() << tobs(sysAff) << "  " << sysAff;
  //    qDebug() << tobs(aff) << "  " << aff;
  //    qDebug() << tobs(curAff);

  if (!curAff) {
    CloseHandle(hProcess);
    if (throwOnFail) {
      QString err{QString{"%0 : Invalid processor core affinity mask <%1> | app : <%2> | system : <%3>"}.arg(
          __PRETTY_FUNCTION__, tobs(aff), tobs(appAff), tobs(sysAff))};
      return lambdaFail(err);
    }
    return false;
  }

  auto success{SetProcessAffinityMask(hProcess, curAff)};
  CloseHandle(hProcess);

  if (!success) {
    QString err{
        QString{"%0 : Can't set processor core affinity to <%1> cores"}.arg(__PRETTY_FUNCTION__).arg(coreCountToUse)};
    return lambdaFail(err);
  }

  if (verbose) {
    SPDLOG_INFO("Successfully set core count affinity to <{}> cores", coreCountToUse);
  }

  return true;
}

unsigned int ScancodeFromVirtualKey(unsigned int virtualkey, unsigned int alias) {
  int scancode = 0;
  if (virtualkey == VK_PAUSE) {
    // MapVirtualKey does not work with VK_PAUSE
    scancode = 0x45;
  } else {
    scancode = MapVirtualKey(virtualkey, MAPVK_VK_TO_VSC);
  }

  constexpr auto EXTENDED_FLAG = 0x100;
  switch (virtualkey) {
    case VK_LEFT:
    case VK_UP:
    case VK_RIGHT:
    case VK_DOWN:  // arrow keys
    case VK_PRIOR:
    case VK_NEXT:  // page up and page down
    case VK_END:
    case VK_HOME:
    case VK_INSERT:
    case VK_DELETE:
    case VK_DIVIDE:  // numpad slash
    case VK_NUMLOCK:
    case VK_RCONTROL:
    case VK_RMENU: {
      scancode |= EXTENDED_FLAG;  // set extended bit
      break;
    }
    case VK_RETURN: {
      // Remove ambiguity between Enter and Numpad Enter.
      // In Windows, VK_RETURN is used for both.
      if (alias == Qt::Key_Enter) {
        scancode |= EXTENDED_FLAG;  // set extended bit
        break;
      }
    }
  }

  return scancode;
}

QString VkCodeToStr(int32_t keyCode) {
  switch (keyCode) {
    case VK_LBUTTON:
      return QObject::tr("Left Mouse Button");
    case VK_RBUTTON:
      return QObject::tr("Right Mouse Button");
    case VK_CANCEL:
      return QObject::tr("Control-break Processing");
    case VK_MBUTTON:
      return QObject::tr("Middle Mouse Button");
    case VK_XBUTTON1:
      return QObject::tr("X1 Mouse Button");
    case VK_XBUTTON2:
      return QObject::tr("X2 Mouse Button");
    case VK_BACK:
      return QObject::tr("Backspace");
    case VK_TAB:
      return QObject::tr("Tab");
    case VK_CLEAR:
      return QObject::tr("Clear");
    case VK_RETURN:
      return QObject::tr("Enter");
    case VK_SHIFT:
      return QObject::tr("Shift");
    case VK_CONTROL:
      return QObject::tr("Control");
    case VK_MENU:
      return QObject::tr("Alt");
    case VK_PAUSE:
      return QObject::tr("Pause");
    case VK_CAPITAL:
      return QObject::tr("Caps Lock");
    case VK_ESCAPE:
      return QObject::tr("Escape");
    case VK_SPACE:
      return QObject::tr("Space");
    case VK_PRIOR:
      return QObject::tr("Page Up");
    case VK_NEXT:
      return QObject::tr("Page Down");
    case VK_END:
      return QObject::tr("End");
    case VK_HOME:
      return QObject::tr("Home");
    case VK_LEFT:
      return QObject::tr("Left Arrow");
    case VK_UP:
      return QObject::tr("Up Arrow");
    case VK_RIGHT:
      return QObject::tr("Right Arrow");
    case VK_DOWN:
      return QObject::tr("Down Arrow");
    case VK_SELECT:
      return QObject::tr("Select");
    case VK_PRINT:
      return QObject::tr("Print");
    case VK_EXECUTE:
      return QObject::tr("Execute");
    case VK_SNAPSHOT:
      return QObject::tr("Print Screen");
    case VK_INSERT:
      return QObject::tr("Insert");
    case VK_DELETE:
      return QObject::tr("Delete");
    case VK_HELP:
      return QObject::tr("Help");
    case VK_LWIN:
      return QObject::tr("Left Windows");
    case VK_RWIN:
      return QObject::tr("Right Windows");
    case VK_APPS:
      return QObject::tr("Applications");
    case VK_SLEEP:
      return QObject::tr("Sleep");
    case VK_NUMPAD0:
      return QObject::tr("Numpad 0");
    case VK_NUMPAD1:
      return QObject::tr("Numpad 1");
    case VK_NUMPAD2:
      return QObject::tr("Numpad 2");
    case VK_NUMPAD3:
      return QObject::tr("Numpad 3");
    case VK_NUMPAD4:
      return QObject::tr("Numpad 4");
    case VK_NUMPAD5:
      return QObject::tr("Numpad 5");
    case VK_NUMPAD6:
      return QObject::tr("Numpad 6");
    case VK_NUMPAD7:
      return QObject::tr("Numpad 7");
    case VK_NUMPAD8:
      return QObject::tr("Numpad 8");
    case VK_NUMPAD9:
      return QObject::tr("Numpad 9");
    case VK_MULTIPLY:
      return QObject::tr("Multiply");
    case VK_ADD:
      return QObject::tr("Add");
    case VK_SEPARATOR:
      return QObject::tr("Separator");
    case VK_SUBTRACT:
      return QObject::tr("Subtract");
    case VK_DECIMAL:
      return QObject::tr("Decimal");
    case VK_DIVIDE:
      return QObject::tr("Divide");
    case VK_F1:
      return QObject::tr("F1");
    case VK_F2:
      return QObject::tr("F2");
    case VK_F3:
      return QObject::tr("F3");
    case VK_F4:
      return QObject::tr("F4");
    case VK_F5:
      return QObject::tr("F5");
    case VK_F6:
      return QObject::tr("F6");
    case VK_F7:
      return QObject::tr("F7");
    case VK_F8:
      return QObject::tr("F8");
    case VK_F9:
      return QObject::tr("F9");
    case VK_F10:
      return QObject::tr("F10");
    case VK_F11:
      return QObject::tr("F11");
    case VK_F12:
      return QObject::tr("F12");
  }

  static bool firstCall{true};
  static bool canProceed{false};

  static HKL lpList[2];
  static HKL kb;
  static BYTE uKeyboardState[256];

  if (firstCall) {
    auto kbCount = GetKeyboardLayoutList(2, lpList);
    if (kbCount != 0) {
      canProceed = true;
    } else {
      return {};
    }
    kb = lpList[0];

    // Initialization of KeyBoardState
    for (int i = 0; i < 256; ++i) {
      uKeyboardState[i] = 0;
    }
  }
  if (!canProceed) return {};

  auto ScanCode = MapVirtualKeyExW(keyCode, MAPVK_VK_TO_VSC, kb);
  auto VKCode2 = MapVirtualKeyExW(ScanCode, MAPVK_VSC_TO_VK, kb);
  TCHAR ch1 = MapVirtualKeyExW(VKCode2, MAPVK_VK_TO_CHAR, kb);
  std::ignore = ch1;

  TCHAR buffer[1024];
  auto charCount = ToUnicodeEx(keyCode, ScanCode, uKeyboardState, buffer, 1024, 0, kb);
  if (charCount < 1) {
    return {};
  } else if (charCount > 2) {
    QString out{};
    for (int i{}; i < charCount; ++i) {
      out.append(buffer[i]);
    }
    return out;
  }

  return QString{buffer[0]};
}

bool IsNumLockEnabled() {
  // Get the state of the Num Lock key
  return (GetKeyState(VK_NUMLOCK) & 0x0001) != 0;
}

void SetNumLock(bool enable) {
  // Check the current state of Num Lock
  const bool kNumLockEnabled{IsNumLockEnabled()};

  // If the current state is different from the desired state, simulate a key press
  if (kNumLockEnabled != enable) {
    // Simulate a key press
    keybd_event(VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
    keybd_event(VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
  }
}

}  // namespace win
