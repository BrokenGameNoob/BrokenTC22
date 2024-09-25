#pragma once

#include <QStandardPaths>

#include <DataStructures/games.hpp>

namespace btc2::path {
namespace subpaths {
constexpr auto kControllerProfiles{"ControllersProfiles"};
constexpr auto kGamesProfiles{"GamesProfiles"};

constexpr auto kKeyboardProfileFile{"keyboard_profile.json"};
constexpr auto kApplicationSettingsFile{"application_settings.json"};
}  // namespace subpaths

QString GetAppDataPath();

QString GetControllerProfilesPath();
QString GetControllerProfilePath(const QString& profile_name);

QString GetGamesProfilesPath();
QString GetGameProfilePath(Game game);

QString GetKeyboardProfilePath();
QString GetApplicationSettingsPath();

}  // namespace btc2::path
