#pragma once

#include <QObject>

#include <DataStructures/games.hpp>
#include <DataStructures/structures.hpp>
#include <DataStructures/structures_utils.hpp>

namespace btc2 {

class GameProfilesHandler : public QObject {
  Q_OBJECT

  Q_PROPERTY(QVariant gameProfile READ GetQMLGameProfile NOTIFY currentGameChanged FINAL);

 signals:
  void currentGameChanged();
  void profileUpdated();

 public:
  static void Init();

 public:
  explicit GameProfilesHandler();

  QVariant GetQMLGameProfile() const;

  std::shared_ptr<GameProfileTheCrew> GetTheCrew2Profile() const {
    return m_the_crew2_profile;
  }
  std::shared_ptr<GameProfileTheCrew> GetTheCrewMotorfistProfile() const {
    return m_the_crew_motorfist_profile;
  }

  std::shared_ptr<GameProfileTheCrew> GeTheCrewProfile(Game::Types game) {
    switch (game) {
      case Game::THE_CREW_2:
        return m_the_crew2_profile;
      case Game::THE_CREW_MOTORFIST:
        return m_the_crew_motorfist_profile;
      default:
        return nullptr;
    }
    return nullptr;
  }

  void SetCurrentGame(Game::Types game) {
    m_current_game = game;
    emit currentGameChanged();
  }

  ConflictsResults UpdateConflicts(const KeyboardProfile& keyboard_profile);

 private:
  std::shared_ptr<GameProfileTheCrew> m_the_crew2_profile{nullptr};
  std::shared_ptr<GameProfileTheCrew> m_the_crew_motorfist_profile{nullptr};

  Game::Types m_current_game{Game::NONE};
};

}  // namespace btc2
