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

QString GetKeyboardProfilePath() {
  return QString("%0/%1").arg(GetAppDataPath(), path::subpaths::kKeyboardProfileFile);
}
QString GetApplicationSettingsPath() {
  return QString("%0/%1").arg(GetAppDataPath(), path::subpaths::kApplicationSettingsFile);
}

}  // namespace btc2::path
