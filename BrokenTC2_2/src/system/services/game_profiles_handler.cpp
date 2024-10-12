#include "game_profiles_handler.hpp"

#include <DataStructures/path_utils.hpp>
#include <utils/qt_utils.hpp>

namespace btc2 {

void GameProfilesHandler::Init() {
  CREGISTER_QML_UNCREATABLE_TYPE(btc2, GameProfilesHandler, "CppOwned");
}

GameProfilesHandler::GameProfilesHandler()
    : QObject{nullptr},
      m_the_crew2_profile{std::make_shared<GameProfileTheCrew>(path::GetGameProfilePath(Game::THE_CREW_2), nullptr)},
      m_the_crew_motorfist_profile{
          std::make_shared<GameProfileTheCrew>(path::GetGameProfilePath(Game::THE_CREW_MOTORFIST), nullptr)} {}

QVariant GameProfilesHandler::GetQMLGameProfile() const {
  switch (m_current_game) {
    case Game::THE_CREW_2:
      return QVariant::fromValue(m_the_crew2_profile.get());
    case Game::THE_CREW_MOTORFIST:
      return QVariant::fromValue(m_the_crew_motorfist_profile.get());
    default:
      SPDLOG_ERROR("Game profile not found for game: <{}>\nDefaulting to BT2", static_cast<int>(m_current_game));
      return QVariant::fromValue(m_the_crew2_profile.get());
  }
}

}  // namespace btc2
