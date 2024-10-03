#include "DataStructures/games.hpp"

#include <QJsonObject>

#include <Logger/logger.hpp>
#include "qqml.h"

namespace btc2 {

const std::map<Games, QString> GameSelector::kGameNames{
                                                        {Game::NONE, "Unkown"}, {Game::THE_CREW_2, "The Crew 2"}, {Game::THE_CREW_MOTORFIST, "The Crew Motorfist"}};

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

Games GameSelector::GetGameFromName(const QString& name) {
  for (const auto& [game, game_name] : GameSelector::kGameNames) {
    if (game_name == name) {
      return game;
    }
  }
  return Game::NONE;
}

}  // namespace btc2
