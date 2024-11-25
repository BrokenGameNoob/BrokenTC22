#include "DataStructures/games.hpp"

#include <QJsonObject>

#include <Logger/logger.hpp>

#include "qqml.h"

namespace btc2 {

const GameInfo& GetGameInfo(Game::Types game) {
  const auto& infos{GetGameInfoList()};
  if (infos.contains(game)) {
    return infos.at(game);
  }
  return infos.at(Game::NONE);
}

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
  for (const auto& [game, info] : GetGameInfoList()) {
    if (game == Game::NONE) {
      continue;
    }
    out.append(info.kGameName);
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
  for (const auto& [game, info] : GetGameInfoList()) {
    if (info.kGameName == name) {
      return game;
    }
  }
  return Game::NONE;
}

QString GameSelector::GetGameName(Game::Types game) {
  return GetGameInfo(game).kGameName;
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
