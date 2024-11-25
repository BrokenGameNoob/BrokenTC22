#pragma once

#include <QString>

#include <DataStructures/games.hpp>
#include <DataStructures/structures.hpp>
#include <WinUtils/utils.hpp>

namespace btc2 {

inline QString GetConfigPath(const GameInfo& gameInfo) {
  return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + gameInfo.kFolderInDocuments;
}

inline bool IsGameRunning(const GameInfo& gameInfo) {
  return win::IsProcessRunning(gameInfo.kProcessName);
}

inline bool KillGame(const GameInfo& gameInfo) {
  auto pId{win::FindProcessId(gameInfo.kProcessName)};
  if (!pId) {
    return true;
  }

  return win::TerminateProcess(pId.value());
}

QStringList GetBindingsFiles(const QString& configDirPath);
QString GetBindingKeyboardFile(const QString& configDirPath);

namespace xml {

/*!
 * \brief Edit a TC2 xml config file to remove GearUp and GearDown key
 * \param xmlPath : file to edit
 * \return success
 */
bool EditXmlControllerConf(const QString& xmlPath);

bool EditXmlKeyboardConf(const QString& xmlPath, GameProfileTheCrew* settings);

}  // namespace xml
}  // namespace btc2
