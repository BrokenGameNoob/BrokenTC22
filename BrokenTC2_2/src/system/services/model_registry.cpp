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

std::shared_ptr<ControllerProfile> ModelRegistry::GetControllerProfile(const QString& controller_name) {
  auto& m_controller_profiles = I().m_controller_profiles;

  const auto kControllerProfilePath{path::GetControllerProfilePath(controller_name)};
  if (auto iter{m_controller_profiles.find(kControllerProfilePath)};
      iter != m_controller_profiles.end() && !iter->second.expired()) {
    SPDLOG_DEBUG("[ModelRegistry] Returning existing controller profile for <{}>", controller_name);
    return m_controller_profiles[kControllerProfilePath].lock();
  }

  SPDLOG_DEBUG("[ModelRegistry] NEW controller profile for <{}>", controller_name);
  auto controller_profile{std::make_shared<ControllerProfile>(kControllerProfilePath, nullptr)};
  controller_profile->SetName(controller_name);
  m_controller_profiles[kControllerProfilePath] = controller_profile;
  return controller_profile;
}

}  // namespace btc2
