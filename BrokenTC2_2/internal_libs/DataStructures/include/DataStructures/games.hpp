#pragma once

#include <QObject>
#include <qobjectdefs.h>
#include <qtmetamacros.h>

#include <Logger/logger.hpp>
#include <Utils/macro_define.hpp>

#include "qqml.h"

namespace btc2 {
Q_NAMESPACE

struct GameInfo {
  const QString kGameName{};
  const QString kFolderInDocuments{};
  const QString kProcessName{};
};

class Game : public QObject {
  Q_OBJECT
 public:
  enum Types {
    NONE = 0b1 << 0,
    THE_CREW_2 = 0b1 << 1,
    THE_CREW_MOTORFIST = 0b1 << 2,
    ALL = ~0,
  };
  Q_ENUM(Types)
};

Q_DECLARE_FLAGS(Games, Game::Types)
Q_DECLARE_OPERATORS_FOR_FLAGS(Games)

static const auto& GetGameInfoList() {
  static const std::map<Game::Types, GameInfo> kGamesInfo{
      {Game::Types::NONE, {"Unknown", "", ""}},
      {Game::Types::THE_CREW_2, {"The Crew 2", "The Crew 2", "TheCrew2.exe"}},
      {Game::Types::THE_CREW_MOTORFIST, {"The Crew Motorfist", "TheCrewMotorfest", "TheCrewMotorfest.exe"}}};
  return kGamesInfo;
}
const GameInfo& GetGameInfo(Game::Types game);

Game::Types GetFocusedGameFromWindowTitle(const QString& title);

class GameSelector : public QObject {
  Q_OBJECT

  Q_PROPERTY(Game::Types selectedGame READ GetSelectedGame WRITE SetSelectedGame NOTIFY gameChanged)
  Q_PROPERTY(QString selectedGameName READ GetSelectedGameName NOTIFY gameChanged)
  Q_PROPERTY(QString selectionModelSelectedGameName READ GetSelectedGameName NOTIFY selectionModelGameUpdated)

 signals:
  void gameChanged();
  void selectionModelGameUpdated();

 public:
  GameSelector(QObject* parent = nullptr);

  void SetSelectedGame(Games game) {
    const auto kOldGame{m_selected_game};
    m_selected_game = static_cast<Game::Types>(game.toInt());
    if (kOldGame != m_selected_game) {
      SPDLOG_DEBUG("Game changed!");
      emit gameChanged();
    }
  }
  Q_INVOKABLE void SetSelectedGameFromName(const QString& name) {
    SetSelectedGame(GetGameFromName(name));
  }

  Game::Types GetSelectedGame() const {
    return m_selected_game;
  }

  static QStringList GetAvailableGamesNames();
  static QString GetAutoModeStr();
  Q_INVOKABLE static QStringList GameSelectionModel(bool add_auto_option);
  Q_INVOKABLE void SetSelectionModelSelectedGame(const QString& selection_model_name);
  QString GetSelectionModelSelectedGame();

  Q_INVOKABLE static Game::Types GetGameFromName(const QString& name);
  Q_INVOKABLE static QString GetGameName(Game::Types game);

  QString GetSelectedGameName() const {
    return GetGameName(m_selected_game);
  }

 public slots:
  void OnFocusedWindowChanged(Game::Types game);

 private:
  Game::Types m_selected_game{Game::NONE};
  QString m_selection_model_selected_game_name;
  bool m_auto_selection{true};
  bool m_force_auto_selection{true};
};

inline void RegisterGameEnum() {
  // qmlRegisterUncreatableMetaObject(Game::staticMetaObject,"btc2", 1, 0, "Game", "Enum type");
  qmlRegisterUncreatableType<Game>("btc2", 1, 0, "Game", "Enum type");
  qmlRegisterUncreatableType<btc2::GameSelector>("btc2", 1, 0, "GameSelector", "Doesn't need it");
}

}  // namespace btc2
