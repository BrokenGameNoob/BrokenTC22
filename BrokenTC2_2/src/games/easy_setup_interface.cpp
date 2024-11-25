#include "easy_setup_interface.hpp"

#include <Utils/qt_utils.hpp>
#include <games/config_editor.hpp>
#include <system/services/service_manager.hpp>

namespace btc2 {

void InitEasySetupInterface() {
  CREGISTER_QML_TYPE(btc2, EasySetupTheCrew);
}

EasySetupTheCrew::EasySetupTheCrew(QObject* parent) : QObject{parent} {}

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
  m_state_first_iteration = true;
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
      RunHome();
      break;
    case State::OPTIONS:
      RunOptions();
      break;
    case State::WORK_IN_PROGRESS_WAITING_GAME_CLOSING:
      RunWorkInProgressWaitingGameClosing();
      break;
    case State::WORK_IN_PROGRESS_EDITING:
      RunWorkInProgressEditing();
      break;
    case State::FINISHED:
      RunFinished();
      break;
    case State::STATE_MAX_VALUE:
      SPDLOG_ERROR("EasySetupTheCrew should not end up in max value state");
      break;
  }
  m_state_first_iteration = false;
}

void EasySetupTheCrew::RunHome() {
  if (!m_state_first_iteration) {
    SetSuccessState(SuccessState::FAILURE);
    SetState(State::FINISHED);
    return;
  }
  InternalReset();
}

void EasySetupTheCrew::RunOptions() {
  //
}

void EasySetupTheCrew::RunWorkInProgressWaitingGameClosing() {
  const auto& game_info{GetGameInfo(m_game)};
  if (IsGameRunning(game_info)) {
    if (!KillGame(game_info)) {
      SPDLOG_ERROR("Failed to kill game {}", game_info.kGameName);
      SetSuccessState(SuccessState::FAILED_TO_CLOSE_GAME);
      SetState(State::FINISHED);
    }
    QTimer::singleShot(1000, this, [this]() { RunWorkInProgressWaitingGameClosing(); });
    return;
  }
  Next();
}

void EasySetupTheCrew::RunWorkInProgressEditing() {
  if (!m_state_first_iteration) {
    SetSuccessState(SuccessState::FAILURE);
    SetState(State::FINISHED);
    return;
  }

  const auto& game_info{GetGameInfo(m_game)};
  const auto kControllerConfigFileList{GetBindingsFiles(GetConfigPath(game_info))};

  if (kControllerConfigFileList.empty()) {
    SetSuccessState(SuccessState::FAILED_TO_FIND_CONFIG_FILE);
    SetState(State::FINISHED);
    return;
  }

  if (m_auto_set_game_bindings) {
    const auto kKeyboardFile{GetBindingKeyboardFile(GetConfigPath(game_info))};
    if (kKeyboardFile.isEmpty()) {
      SetSuccessState(SuccessState::FAILED_TO_FIND_KEYBOARD_CONFIG_FILE);
      SetState(State::FINISHED);
      return;
    }

    auto game_profile{ServiceManager::I().GetGameProfilesHandler().GeTheCrewProfile(m_game)};
    if (!game_profile) {
      SPDLOG_ERROR("Failed to get game profile instance for {}", game_info.kGameName);
      SetSuccessState(SuccessState::FAILURE);
      SetState(State::FINISHED);
      return;
    }
    if (!xml::EditXmlKeyboardConf(kKeyboardFile, game_profile.get())) {
      SPDLOG_ERROR("Failed to edit keyboard config file {}", kKeyboardFile);
      SetSuccessState(SuccessState::FAILURE);
      SetState(State::FINISHED);
    }
  }

  for (const auto& file : kControllerConfigFileList) {
    if (!xml::EditXmlControllerConf(file)) {
      SPDLOG_ERROR("Failed to edit controller config file {}", file);
      SetSuccessState(SuccessState::FAILURE);
      SetState(State::FINISHED);
      return;
    }
  }

  SetSuccessState(SuccessState::SUCCESS);
  SetState(State::FINISHED);
}

void EasySetupTheCrew::RunFinished() {
  if (!m_state_first_iteration) {
    SetSuccessState(SuccessState::FAILURE);
    SetState(State::FINISHED);
    return;
  }

  emit finished();
}

}  // namespace btc2
