#include "DataStructures/games.hpp"

#include <QJsonObject>

#include <Logger/logger.hpp>

#include "qqml.h"

namespace btc2 {

const std::map<Games, QString> GameSelector::kGameNames{{Game::NONE, QObject::tr("Unkown")},
                                                        {Game::THE_CREW_2, "The Crew 2"},
                                                        {Game::THE_CREW_MOTORFIST, "The Crew Motorfist"}};

Game::Types GetFocusedGameFromWindowTitle(const QString& title) {
  if (title.contains("TheCrew2", Qt::CaseInsensitive)) {
    return Game::THE_CREW_2;
  } else if (title.contains("TheCrewMotorfest", Qt::CaseInsensitive)) {
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

QString GameSelector::GetAutoModeStr() {
  return QObject::tr("Auto");
}

QStringList GameSelector::GameSelectionModel(bool add_auto_option) {
  QStringList out{};
  if (add_auto_option) {
    out += GetAutoModeStr();
  }
  out += GetAvailableGamesNames();
  return out;
}

void GameSelector::SetSelectionModelSelectedGame(const QString& selection_model_name) {
  if (m_force_auto_selection) {
    m_auto_selection = true;
    SPDLOG_DEBUG("Auto selection forced anyway");
  } else {
    if (selection_model_name == GetAutoModeStr()) {
      m_auto_selection = true;
      SPDLOG_DEBUG("Enabled auto mode");
      emit selectionModelGameUpdated();
      return;
    }
    m_auto_selection = false;
    SPDLOG_DEBUG("Disabled auto mode");
  }

  SetSelectedGameFromName(selection_model_name);
  emit selectionModelGameUpdated();
}

QString GameSelector::GetSelectionModelSelectedGame() {
  if (m_auto_selection) {
    return GetAutoModeStr();
  }
  return GetSelectedGameName();
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

void GameSelector::OnFocusedWindowChanged(Game::Types game) {
  if (!m_auto_selection) {
    return;
  }
  if (game == Game::NONE || game == Game::ALL) {
    return;
  }

  SetSelectedGame(game);
}

}  // namespace btc2
