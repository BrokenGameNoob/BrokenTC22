#include "easy_setup_interface.hpp"

#include <Utils/qt_utils.hpp>

namespace btc2 {

void InitEasySetupInterface() {
  CREGISTER_QML_TYPE(btc2, EasySetupTheCrew);
}

EasySetupTheCrew::EasySetupTheCrew(QObject *parent) : QObject{parent} {}

void EasySetupTheCrew::SetGame(Game::Types game) {
  if (m_game == game) {
    return;
  }
  m_game = game;
  emit gameChanged();
}
Game::Types EasySetupTheCrew::GetGame() const {
  return m_game;
}

void EasySetupTheCrew::SetState(State state) {
  if (m_state == state) {
    return;
  }
  m_state = state;
  OnStateSet(state);
  emit stateChanged();
}
EasySetupTheCrew::State EasySetupTheCrew::GetState() const {
  return m_state;
}

void EasySetupTheCrew::SetSuccessState(SuccessState success_state) {
  if (m_success_state == success_state) {
    return;
  }
  m_success_state = success_state;
  emit successStateChanged();
}
EasySetupTheCrew::SuccessState EasySetupTheCrew::GetSuccessState() const {
  return m_success_state;
}

void EasySetupTheCrew::SetAutoSetGameBindings(bool auto_set_game_bindings) {
  if (m_auto_set_game_bindings == auto_set_game_bindings) {
    return;
  }

  m_auto_set_game_bindings = auto_set_game_bindings;
  emit optionChanged();
  return;
}
bool EasySetupTheCrew::GetAutoSetGameBindings() const {
  return m_auto_set_game_bindings;
}

void EasySetupTheCrew::Next() {
  const auto kNextState{static_cast<State>(static_cast<int>(GetState()) + 1)};
  if (kNextState >= State::STATE_MAX_VALUE) {
    SetState(State::HOME);
    return;
  }
  SetState(kNextState);
}

void EasySetupTheCrew::Previous() {
  const auto kPreviousState{static_cast<State>(static_cast<int>(GetState()) - 1)};
  if (kPreviousState < State::HOME) {
    return;
  }
  SetState(kPreviousState);
}

void EasySetupTheCrew::Reset() {
  SetState(State::HOME);
}
void EasySetupTheCrew::Start() {
  Reset();
  emit started();
}
void EasySetupTheCrew::Cancel() {
  Reset();
  emit cancelled();
}

void EasySetupTheCrew::InternalReset() {
  m_waiting_for_game_to_be_closed = false;
}

void EasySetupTheCrew::OnStateSet(State state) {
  switch (state) {
    case State::HOME:
      InternalReset();
      break;
    case State::OPTIONS:
      break;
    case State::WORK_IN_PROGRESS_WAITING_GAME_CLOSING:
      break;
    case State::WORK_IN_PROGRESS_EDITING:
      break;
    case State::FINISHED:
      emit finished();
      break;
    case State::STATE_MAX_VALUE:
      SPDLOG_ERROR("EasySetupTheCrew should not end up in max value state");
      break;
  }
}

}  // namespace btc2
