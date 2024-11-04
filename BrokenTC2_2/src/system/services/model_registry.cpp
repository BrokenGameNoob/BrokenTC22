#include "model_registry.hpp"

#include <DataStructures/path_utils.hpp>
#include <utils/qt_utils.hpp>

namespace btc2 {

ModelRegistry::ModelRegistry() : QObject{nullptr} {
  SPDLOG_DEBUG("New INSTANCE of modelregistry");
}

void ModelRegistry::Init() {
  CREGISTER_QML_SINGLETON_I(btc2, ModelRegistry);
}

std::shared_ptr<KeyboardProfile> ModelRegistry::GetKeyboardProfile() {
  const auto kKeyboardProfilePath{path::GetKeyboardProfilePath()};
  auto& m_keyboard_profiles = I().m_keyboard_profiles;

  if (auto iter{m_keyboard_profiles.find(kKeyboardProfilePath)};
      iter != m_keyboard_profiles.end() && !iter->second.expired()) {
    SPDLOG_DEBUG("[ModelRegistry] Returning existing keyboard profile for path <{}>", kKeyboardProfilePath);
    return m_keyboard_profiles[kKeyboardProfilePath].lock();
  }

  SPDLOG_DEBUG("[ModelRegistry] NEW keyboard profile for path <{}>", kKeyboardProfilePath);
  auto keyboard_profile{std::make_shared<KeyboardProfile>(kKeyboardProfilePath, nullptr)};
  m_keyboard_profiles[kKeyboardProfilePath] = keyboard_profile;
  return keyboard_profile;
}

std::shared_ptr<ControllerProfile> ModelRegistry::GetControllerProfileFromPath(const QString& path) {
  auto& m_controller_profiles = I().m_controller_profiles;

  if (auto iter{m_controller_profiles.find(path)}; iter != m_controller_profiles.end() && !iter->second.expired()) {
    SPDLOG_DEBUG("[ModelRegistry] Returning existing controller profile for path <{}>", path);
    return m_controller_profiles[path].lock();
  }

  SPDLOG_DEBUG("[ModelRegistry] NEW controller profile for path <{}>", path);
  auto controller_profile{std::make_shared<ControllerProfile>(path, nullptr)};
  m_controller_profiles[path] = controller_profile;
  return controller_profile;
}

std::shared_ptr<ControllerProfile> ModelRegistry::GetControllerProfile(const QString& controller_name) {
  const auto kControllerProfilePath{path::GetControllerProfilePath(controller_name)};
  auto rval{GetControllerProfileFromPath(kControllerProfilePath)};
  rval->SetName(controller_name);
  return rval;
}

}  // namespace btc2
