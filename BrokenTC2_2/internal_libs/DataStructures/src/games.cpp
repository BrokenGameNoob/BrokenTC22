#include "DataStructures/games.hpp"

#include <QJsonObject>

#include <Logger/logger.hpp>

namespace btc2 {

const std::map<Game, QString> GameSelector::kGameNames{
    {Game::kNone, "Unkown"}, {Game::kTheCrew2, "The Crew 2"}, {Game::kTheCrewMotorfist, "The Crew Motorfist"}};

GameSelector::GameSelector(QObject* parent) : QObject{parent} {
  //
}

QStringList GameSelector::GetAvailableGamesNames() {
  QStringList out{};
  for (const auto& [game, name] : kGameNames) {
    if (game == Game::kNone) {
      continue;
    }
    out.append(name);
  }
  return out;
}

Game GameSelector::GetGameFromName(const QString& name) {
  for (const auto& [game, game_name] : GameSelector::kGameNames) {
    if (game_name == name) {
      return game;
    }
  }
  return Game::kNone;
}

QString GameSelector::GetGameName(Game game) {
  if (kGameNames.find(game) == kGameNames.end()) {
    SPDLOG_ERROR("Game not found: <{}>", static_cast<int>(game));
    return kGameNames.at(Game::kNone);
  }
  return kGameNames.at(game);
}

}  // namespace btc2
