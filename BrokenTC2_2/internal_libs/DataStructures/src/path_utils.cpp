#include <include/DataStructures/path_utils.hpp>

namespace btc2::path {

QString GetAppDataPath() {
  return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
}

QString GetControllerProfilesPath() {
  return QString("%0/%1").arg(GetAppDataPath(), path::subpaths::kControllerProfiles);
}
QString GetControllerProfilePath(const QString& profile_name) {
  return QString("%0/%1.json").arg(GetControllerProfilesPath(), profile_name);
}

QString GetGamesProfilesPath() {
  return QString("%0/%1").arg(GetAppDataPath(), path::subpaths::kGamesProfiles);
}
QString GetGameProfilePath(Game game) {
  return QString("%0/%1.json").arg(GetGamesProfilesPath(), GameSelector::GetGameName(game));
}

QString GetKeyboardProfilePath() {
  return QString("%0/%1").arg(GetAppDataPath(), path::subpaths::kKeyboardProfileFile);
}
QString GetApplicationSettingsPath() {
  return QString("%0/%1").arg(GetAppDataPath(), path::subpaths::kApplicationSettingsFile);
}
QString GetOverlaySettingsPath() {
  return QString("%0/%1").arg(GetAppDataPath(), path::subpaths::kOverlaySettingsFile);
}

}  // namespace btc2::path
