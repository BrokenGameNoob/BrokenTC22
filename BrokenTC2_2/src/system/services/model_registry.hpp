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

  static std::shared_ptr<ControllerProfile> GetControllerProfileFromPath(const QString& path);
  static std::shared_ptr<ControllerProfile> GetControllerProfile(const QString& controller_name);

 private:
  ModelRegistry();
  std::map<QString, std::weak_ptr<ControllerProfile>> m_controller_profiles{};
};

}  // namespace btc2
