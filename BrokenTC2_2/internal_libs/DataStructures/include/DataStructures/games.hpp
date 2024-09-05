#pragma once

#include <QObject>
#include <qobjectdefs.h>
#include <qtmetamacros.h>

#include <Utils/macro_define.hpp>

namespace btc2 {
Q_NAMESPACE

enum class Game {
  kNone = 0b1 << 0,
  kTheCrew2 = 0b1 << 1,
  kTheCrewMotorfist = 0b1 << 2,
};

Q_DECLARE_FLAGS(Games, Game)
Q_DECLARE_OPERATORS_FOR_FLAGS(Games)

class GameSelector : public QObject {
  Q_OBJECT

  Q_PROPERTY(Game selectedGame READ GetSelectedGame WRITE SetSelectedGame NOTIFY gameChanged)
  Q_PROPERTY(QString selectedGameName READ GetSelectedGameName NOTIFY gameChanged)

 signals:
  void gameChanged();

 public:
  GameSelector(QObject* parent = nullptr);

  void SetSelectedGame(Game game) {
    m_selected_game = game;
    emit gameChanged();
  }
  Q_INVOKABLE void SetSelectedGameFromName(const QString& name) {
    m_selected_game = GetGameFromName(name);
    emit gameChanged();
  }

  Game GetSelectedGame() const {
    return m_selected_game;
  }

  Q_INVOKABLE static QStringList GetAvailableGamesNames();
  Q_INVOKABLE static Game GetGameFromName(const QString& name);

  QString GetSelectedGameName() const {
    return kGameNames.at(m_selected_game);
  }

 private:
  static const std::map<Game, QString> kGameNames;

  Game m_selected_game{Game::kNone};
};

}  // namespace btc2
