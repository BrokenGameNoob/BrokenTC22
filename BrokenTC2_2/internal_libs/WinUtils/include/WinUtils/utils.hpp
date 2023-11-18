#ifndef WIN_WINUTILS_HPP
#define WIN_WINUTILS_HPP

#include <QString>
#include <vector>
#include <windows.h>

#include <Logger/logger.hpp>

namespace win {

std::optional<DWORD> FindProcessId(const QString& pName);
std::vector<DWORD> FindProcessesId(const QString& pName);
std::vector<DWORD> GetAllProcesses();

std::string GetProcessName(DWORD processID);

bool TerminateProcess(DWORD processID);
bool TerminateProcess(const QString& p_name);

inline bool IsProcessRunning(const QString& pName) {
  return FindProcessId(pName).has_value();
}

inline bool IsProcessRunning(DWORD pId) {
  auto ids{GetAllProcesses()};
  return std::find(cbegin(ids), cend(ids), pId) != cend(ids);
}

int32_t ProcessCount(const QString& pName);

int32_t GetCoreCount();

bool SetCoreCountAffinity(int32_t coreCountToUse, bool throwOnFail = false, bool verbose = false);

unsigned int ScancodeFromVirtualKey(unsigned int virtualkey, unsigned int alias = 0);
QString VkCodeToStr(int32_t keyCode);

}  // namespace win

#endif  // WIN_WINUTILS_HPP
