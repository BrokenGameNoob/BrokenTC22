#include "controller_handler.hpp"

#include <DataStructures/path_utils.hpp>
#include <Logger/logger.hpp>
#include <QSDL/sdl_event_handler.hpp>
#include <system/services/model_registry.hpp>
#include <system/services/service_manager.hpp>
#include <utils/qt_utils.hpp>

namespace btc2 {

ControllerHandler::ControllerHandler()
    : QObject{nullptr},
      m_game_controller{std::make_unique<qsdl::GameController>(-1, nullptr)},
      m_active_profile{ModelRegistry::GetControllerProfile(tr("Default"))} {
  using qsdl::GameController;
  using qsdl::SDLEventHandler;
  connect(SDLEventHandler::Instance(),
          &SDLEventHandler::gameControllerAdded,
          this,
          &ControllerHandler::OnControllerPluggedIn);
  connect(SDLEventHandler::Instance(),
          &SDLEventHandler::gameControllerRemoved,
          this,
          &ControllerHandler::OnControllerUnplugged);

  connect(m_game_controller.get(), &GameController::buttonDown, this, &ControllerHandler::OnButtonDown);
  connect(m_game_controller.get(), &GameController::buttonUp, this, &ControllerHandler::OnButtonUp);

  qsdl::SDLEventHandler::Start();
  qsdl::SDLEventHandler::RegisterController(m_game_controller.get());

  RefreshKnownControllersFromDisk();
}

void ControllerHandler::Init() {
  CREGISTER_QML_UNCREATABLE_TYPE(btc2, ControllerHandler, "CppOwned");
}

QStringList ControllerHandler::GetControllerList() const {
  return qsdl::GetPluggedJoysticks();
}

QList<ControllerProfile*> ControllerHandler::QMLGetKnownProfiles() const {
  QList<ControllerProfile*> out{};

  out.reserve(m_known_controller_profiles.size());
  for (const auto& profile : m_known_controller_profiles) {
    out.append(profile.get());
  }
  return out;
}

ControllerProfile* ControllerHandler::QMLActiveController() const {
  return m_active_profile.get();
}

void ControllerHandler::SetActiveController(const QString& controller_name) {
  SetActiveController(ModelRegistry::GetControllerProfile(controller_name));
}

void ControllerHandler::SetActiveController(std::shared_ptr<ControllerProfile> controller_profile) {
  if (m_active_profile && controller_profile->Name() == m_active_profile->Name()) {
    return;
  }
  m_old_profile = m_active_profile;
  m_active_profile = std::move(controller_profile);

  emit activeControllerChanged();
}

void ControllerHandler::RefreshKnownControllersFromDisk() {
  const QDir kControllerProfilesDir{path::GetControllerProfilesPath()};
  const auto kControllerProfilesList{kControllerProfilesDir.entryList({"*.json"}, QDir::Files)};

  m_known_controller_profiles.clear();
  for (const auto& controller_profile_path : kControllerProfilesList) {
    m_known_controller_profiles.emplace_back(
        ModelRegistry::GetControllerProfileFromPath(kControllerProfilesDir.absoluteFilePath(controller_profile_path)));
  }
  SortKnownControllers();
}

void ControllerHandler::SortKnownControllers() {
  std::sort(
      m_known_controller_profiles.begin(), m_known_controller_profiles.end(), [](const auto& lhs, const auto& rhs) {
        return lhs->PriorityIndex() == rhs->PriorityIndex() ? lhs->Name() < rhs->Name()
                                                            : lhs->PriorityIndex() < rhs->PriorityIndex();
      });
  emit knownControllersProfilesUpdated();
}

void ControllerHandler::ReorderFromList(QList<ControllerProfile*> new_profile_list) {
  std::vector<std::shared_ptr<ControllerProfile>> new_known_controller_profiles{};
  std::vector<std::shared_ptr<ControllerProfile>> not_found_profiles{};

  for (const auto& profile : new_profile_list) {
    auto found_profile{std::find_if(m_known_controller_profiles.begin(),
                                    m_known_controller_profiles.end(),
                                    [profile](const auto& known_profile) { return known_profile.get() == profile; })};
    if (found_profile == m_known_controller_profiles.end()) {
      SPDLOG_ERROR("Profile from new order from QList: <{}> not found in current known profiles", profile->Name());
      continue;
    }
    new_known_controller_profiles.emplace_back(*found_profile);
  }

  for (const auto& nf : not_found_profiles) {
    new_known_controller_profiles.emplace_back(nf);
  }

  for (size_t i{}; i < new_known_controller_profiles.size(); ++i) {
    new_known_controller_profiles[i]->SetPriorityIndex(i);
  }
  m_known_controller_profiles = new_known_controller_profiles;
  SortKnownControllers();
}

void ControllerHandler::OnControllerPluggedIn(int controller_id) {
  SPDLOG_DEBUG("Controller plugged in: <{}>", controller_id);
  auto plugged_in_profile{ModelRegistry::GetControllerProfile(SDL_JoystickNameForIndex(controller_id))};
  if (m_active_profile) {
    if (m_active_profile->PriorityIndex() >= plugged_in_profile->PriorityIndex()) {
      SetActiveController(plugged_in_profile->Name());
    }
  }

  if (std::find(m_known_controller_profiles.begin(), m_known_controller_profiles.end(), plugged_in_profile) ==
      m_known_controller_profiles.end()) {
    m_known_controller_profiles.emplace_back(plugged_in_profile);
    SortKnownControllers();
  }

  m_game_controller->ConnectController(controller_id);

  emit controllerPluggedInOrOut();
}
void ControllerHandler::OnControllerUnplugged(int controller_id) {
  SPDLOG_DEBUG("Controller unplugged: <{}>", controller_id);
  m_game_controller->DisconnectController(false);
  emit controllerPluggedInOrOut();
}

void ControllerHandler::OnButtonDown(int button) {
  SPDLOG_DEBUG("Button down from <{}>: <{}>", m_game_controller->Id(), button);
}
void ControllerHandler::OnButtonUp(int button) {
  SPDLOG_DEBUG("Button up from <{}>: <{}>", m_game_controller->Id(), button);
}

}  // namespace btc2
