#pragma once

#include <QObject>
#include <qobjectdefs.h>
#include <qtmetamacros.h>

#include <Utils/macro_define.hpp>

#include "qqml.h"

namespace btc2 {
Q_NAMESPACE

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

inline void RegisterGameEnum() {
  // qmlRegisterUncreatableMetaObject(Game::staticMetaObject,"btc2", 1, 0, "Game", "Enum type");
  qmlRegisterUncreatableType<Game>("btc2", 1, 0, "Game", "Enum type");
}

Game::Types GetFocusedGameFromWindowTitle(const QString& title);

class GameSelector : public QObject {
  Q_OBJECT

  Q_PROPERTY(Game::Types selectedGame READ GetSelectedGame WRITE SetSelectedGame NOTIFY gameChanged)
  Q_PROPERTY(QString selectedGameName READ GetSelectedGameName NOTIFY gameChanged)

 signals:
  void gameChanged();

 public:
  GameSelector(QObject* parent = nullptr);

  void SetSelectedGame(Games game) {
    m_selected_game = static_cast<Game::Types>(game.toInt());
    emit gameChanged();
  }
  Q_INVOKABLE void SetSelectedGameFromName(const QString& name) {
    m_selected_game = static_cast<Game::Types>(GetGameFromName(name).toInt());
    emit gameChanged();
  }

  Game::Types GetSelectedGame() const {
    return m_selected_game;
  }

  Q_INVOKABLE static QStringList GetAvailableGamesNames();
  Q_INVOKABLE static Game GetGameFromName(const QString& name);
  Q_INVOKABLE static QString GetGameName(Game game);

  QString GetSelectedGameName() const {
    return kGameNames.at(m_selected_game);
  }

 private:
  static const std::map<Games, QString> kGameNames;

  Game::Types m_selected_game{Game::THE_CREW_2};
};

}  // namespace btc2
