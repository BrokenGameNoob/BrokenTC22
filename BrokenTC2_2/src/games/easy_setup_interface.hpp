#include <QCoreApplication>
#include <QObject>
#include <QQmlEngine>

#include <DataStructures/games.hpp>

namespace btc2 {

class EasySetupTheCrew : public QObject {
  Q_OBJECT

  Q_PROPERTY(Game::Types game READ GetGame WRITE SetGame NOTIFY gameChanged)
  Q_PROPERTY(State state READ GetState WRITE SetState NOTIFY stateChanged)
  Q_PROPERTY(bool autoSetGameBindings READ GetAutoSetGameBindings WRITE SetAutoSetGameBindings NOTIFY optionChanged)

 signals:
  void gameChanged();
  void stateChanged();
  void optionChanged();

  void cancelled();
  void started();
  void finished();

 public:
  enum SuccessState { UNKNOWN_SUCCESS_STATE, SUCCESS, FAILURE };
  Q_ENUM(SuccessState)

  enum State {
    HOME,
    OPTIONS,
    WORK_IN_PROGRESS_WAITING_GAME_CLOSING,
    WORK_IN_PROGRESS_EDITING,
    FINISHED,
    STATE_MAX_VALUE
  };
  Q_ENUM(State)

 public:
  EasySetupTheCrew(QObject* parent = nullptr);

  void SetGame(Game::Types game);
  Game::Types GetGame() const;

  void SetState(State state);
  State GetState() const;

  void SetAutoSetGameBindings(bool auto_set_game_bindings);
  bool GetAutoSetGameBindings() const;

  Q_INVOKABLE void Next();
  Q_INVOKABLE void Previous();

  Q_INVOKABLE void Reset();
  Q_INVOKABLE void Start();
  Q_INVOKABLE void Cancel();

 private:
  void InternalReset();
  void OnStateSet(State state);

 private:
  Game::Types m_game{Game::NONE};
  State m_state{HOME};

  bool m_auto_set_game_bindings{true};

  std::atomic_bool m_waiting_for_game_to_be_closed{};
};

void InitEasySetupInterface();
Q_COREAPP_STARTUP_FUNCTION(InitEasySetupInterface);

}  // namespace btc2
