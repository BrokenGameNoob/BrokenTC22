#include "controller_handler.hpp"

#include <DataStructures/path_utils.hpp>
#include <Logger/logger.hpp>
#include <QSDL/sdl_event_handler.hpp>
#include <system/services/model_registry.hpp>
#include <system/services/service_manager.hpp>
#include <utils/qt_utils.hpp>
#include <utils/shared_constants.hpp>

namespace btc2 {

ControllerHandler::ControllerHandler()
    : QObject{nullptr},
      m_game_controller{std::make_unique<qsdl::GameController>(-1, nullptr)},
      m_active_profile{ModelRegistry::GetControllerProfile(kDefaultControllerProfileName)} {
  m_active_profile->SetPriorityIndex(std::numeric_limits<int>::max());
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

int ControllerHandler::ControllerIdForName(const QString& controller_name) const {
  return qsdl::FindJoystickByName(controller_name);
}

void ControllerHandler::SetActiveController(const QString& controller_name) {
  SetActiveController(ModelRegistry::GetControllerProfile(controller_name));
}

void ControllerHandler::SetActiveController(int controller_id) {
  SetActiveController(SDL_JoystickNameForIndex(controller_id));
  m_game_controller->ConnectController(controller_id);
}

void ControllerHandler::SetActiveController(std::shared_ptr<ControllerProfile> controller_profile) {
  SPDLOG_DEBUG("Setting PROFILE ACTIVE={}", controller_profile->Name());
  if (m_active_profile && controller_profile->Name() == m_active_profile->Name()) {
    return;
  }
  m_old_profile = m_active_profile;
  disconnect(m_old_profile.get(), nullptr, this, nullptr);
  m_active_profile = std::move(controller_profile);

  auto id = ControllerIdForName(m_active_profile->Name());
  m_game_controller->ConnectController(id);

  std::weak_ptr active_profile_weak{m_active_profile};
  connect(m_active_profile.get(), &ControllerProfile::dataChanged, this, [active_profile_weak]() {
    if (auto active_profile = active_profile_weak.lock()) {
      ServiceManager::I().UpdateSDLAxisThreshold(active_profile->DeadZone());
    }
  });
  ServiceManager::I().UpdateSDLAxisThreshold(m_active_profile->DeadZone());

  emit activeControllerChanged();
}

void ControllerHandler::AutoSetActivePrefferedController() {
  const auto kControllerList{GetControllerList()};
  if (kControllerList.empty()) {
    SetActiveController(kDefaultControllerProfileName);
    return;
  }
  /* else */
  for (const auto& controller : m_known_controller_profiles) {
    if (kControllerList.contains(controller->Name())) {
      SetActiveController(controller);
      return;
    }
  }
  SetActiveController(kDefaultControllerProfileName);
}

void ControllerHandler::RefreshKnownControllersFromDisk() {
  const QDir kControllerProfilesDir{path::GetControllerProfilesPath()};
  const auto kControllerProfilesList{kControllerProfilesDir.entryList({"*.json"}, QDir::Files)};

  m_known_controller_profiles.clear();
  for (const auto& controller_profile_path : kControllerProfilesList) {
    auto profile{
        ModelRegistry::GetControllerProfileFromPath(kControllerProfilesDir.absoluteFilePath(controller_profile_path))};
    if (profile->Name() == kDefaultControllerProfileName) {
      continue;
    }
    m_known_controller_profiles.emplace_back(std::move(profile));
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

  emit controllerPluggedInOrOut();
}
void ControllerHandler::OnControllerUnplugged(int controller_id) {
  SPDLOG_DEBUG("Controller unplugged: <{}>", controller_id);
  if (m_game_controller->Id() == controller_id) {
    m_game_controller->DisconnectController(false);

    AutoSetActivePrefferedController();
  }
  emit controllerPluggedInOrOut();
}

void ControllerHandler::EnterKeybindMode() {
  m_is_in_enter_keybind_mode = true;
  emit enterKeybindModeChanged();
}

void ControllerHandler::LeaveKeybindMode() {
  m_is_in_enter_keybind_mode = false;
  emit enterKeybindModeChanged();
}

bool ControllerHandler::GetIsInEnterKeybindMode() const {
  return m_is_in_enter_keybind_mode;
}

void ControllerHandler::OnButtonDown(int button) {
  emit buttonDown(button);

  if (GetIsInEnterKeybindMode()) {
    return;
  }

  if (!m_active_profile) {
    SPDLOG_ERROR("Active profile not set");
    return;
  }

  if (button == m_active_profile->GearUp()) {
    ServiceManager::GetGearHandler().GearUp();
  } else if (button == m_active_profile->GearDown()) {
    ServiceManager::GetGearHandler().GearDown();
  } else if (button == m_active_profile->SwitchGearMode()) {
    ServiceManager::GetGearHandler().CycleMode();
  } else if (button == m_active_profile->EnableDisableInputs()) {
    ServiceManager::GetGearHandler().SetUserEnabled(!ServiceManager::GetGearHandler().IsUserEnabled());
  } else if (button == m_active_profile->GearReverse()) {
    ServiceManager::GetGearHandler().SetGear(-1);
  } else if (button == m_active_profile->Gear1()) {
    ServiceManager::GetGearHandler().SetGear(1);
  } else if (button == m_active_profile->Gear2()) {
    ServiceManager::GetGearHandler().SetGear(2);
  } else if (button == m_active_profile->Gear3()) {
    ServiceManager::GetGearHandler().SetGear(3);
  } else if (button == m_active_profile->Gear4()) {
    ServiceManager::GetGearHandler().SetGear(4);
  } else if (button == m_active_profile->Gear5()) {
    ServiceManager::GetGearHandler().SetGear(5);
  } else if (button == m_active_profile->Gear6()) {
    ServiceManager::GetGearHandler().SetGear(6);
  } else if (button == m_active_profile->Gear7()) {
    ServiceManager::GetGearHandler().SetGear(7);
  }
}
void ControllerHandler::OnButtonUp(int button) {
  emit buttonUp(button);

  if (GetIsInEnterKeybindMode()) {
    return;
  }
}

}  // namespace btc2
