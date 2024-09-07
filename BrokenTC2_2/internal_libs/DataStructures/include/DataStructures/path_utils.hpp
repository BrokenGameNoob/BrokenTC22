#pragma once

#include <QStandardPaths>

namespace btc2::path {
namespace subpaths {
constexpr auto kControllerProfiles{"ControllersProfiles"};

constexpr auto kKeyboardProfileFile{"keyboard_profile.json"};
constexpr auto kApplicationSettingsFile{"application_settings.json"};
}  // namespace subpaths

QString GetAppDataPath();

QString GetControllerProfilesPath();
QString GetControllerProfilePath(const QString& profile_name);

QString GetKeyboardProfilePath();
QString GetApplicationSettingsPath();

}  // namespace btc2::path
