#include "DataStructures/games.hpp"

#include <QJsonObject>

#include <Logger/logger.hpp>

#include "qqml.h"

namespace btc2 {

const std::map<Games, QString> GameSelector::kGameNames{
    {Game::NONE, "Unkown"}, {Game::THE_CREW_2, "The Crew 2"}, {Game::THE_CREW_MOTORFIST, "The Crew Motorfist"}};

Game::Types GetFocusedGameFromWindowTitle(const QString& title) {
  if (title.contains("The Crew 2", Qt::CaseInsensitive)) {
    return Game::THE_CREW_2;
  } else if (title.contains("The Crew Motorfest", Qt::CaseInsensitive)) {
    return Game::THE_CREW_MOTORFIST;
  }
  return Game::NONE;
}

GameSelector::GameSelector(QObject* parent) : QObject{parent} {
  //
}

QStringList GameSelector::GetAvailableGamesNames() {
  QStringList out{};
  for (const auto& [game, name] : kGameNames) {
    if (game == Game::NONE) {
      continue;
    }
    out.append(name);
  }
  return out;
}

Game::Types GameSelector::GetGameFromName(const QString& name) {
  for (const auto& [game, game_name] : GameSelector::kGameNames) {
    if (game_name == name) {
      return static_cast<Game::Types>(game.toInt());
    }
  }
  return Game::NONE;
}

QString GameSelector::GetGameName(Game::Types game) {
  if (kGameNames.find(game) == kGameNames.end()) {
    SPDLOG_ERROR("Game not found: <{}>", static_cast<int>(game));
    return kGameNames.at(Game::NONE);
  }
  return kGameNames.at(game);
}

}  // namespace btc2
