#pragma once

#include <QObject>
#include <memory>

#include <DataStructures/structures.hpp>

namespace btc2 {

class ModelRegistry : public QObject {
  Q_OBJECT
  QML_SINGLETON
 public:
  static ModelRegistry& I() {
    static auto instance{ModelRegistry()};
    return instance;
  }
  static void Init();

  static std::shared_ptr<KeyboardProfile> GetKeyboardProfile();

  static std::shared_ptr<ControllerProfile> GetControllerProfileFromPath(const QString& path);
  static std::shared_ptr<ControllerProfile> GetControllerProfile(const QString& controller_name);

  static std::shared_ptr<GameOverlayData> GetOverlayProfile(const QString& screen_name);

 private:
  ModelRegistry();
  std::map<QString, std::weak_ptr<ControllerProfile>> m_controller_profiles{};
  std::map<QString, std::weak_ptr<GameOverlayData>> m_overlay_profiles{};

  std::map<QString, std::weak_ptr<KeyboardProfile>> m_keyboard_profiles{};
};

}  // namespace btc2
