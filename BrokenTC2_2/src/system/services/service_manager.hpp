#pragma once

#include <QObject>
#include <QQmlEngine>
#include <git_version.hpp>

#include <Logger/btype.hpp>
#include <QSDL/game_controller.hpp>
#include <games/gear_handler_factory.hpp>
#include <games/gear_handler_the_crew.hpp>
#include <system/services/controller_handler.hpp>

#include "testProto.qpb.h"

namespace btc2 {

class ServiceManager : public QObject {
  Q_OBJECT
  QML_SINGLETON

  Q_PROPERTY(bool hasDebInfo READ HasDebInfo CONSTANT FINAL);
  Q_PROPERTY(QString versionStr READ GetVersionStr CONSTANT FINAL);
  Q_PROPERTY(QStringList availableGearHandlers READ GetAvailableGearHandlers CONSTANT FINAL);
  Q_PROPERTY(BaseGearHandler* gearHandler READ GetRawGearHandler CONSTANT FINAL);
  Q_PROPERTY(ControllerHandler* controllerHandler READ GetRawControllerHandler CONSTANT FINAL);
  Q_PROPERTY(btc2::WarningNotification tmp MEMBER m_tmp);

 signals:
  void gearHandlerChanged();

 public:
  ~ServiceManager();
  static ServiceManager& I() {
    static auto instance{ServiceManager()};
    return instance;
  }

  static void Init();

  /* Test function */
  Q_INVOKABLE void test();  // NOLINT

  /* Version related */
  QString GetVersionStr() const {
    return QString::fromStdString(gitversion::GetVersionRepresentationString());
  }

  /* Is built in debug ? */
  bool HasDebInfo() const {
    return btype::HasDebInfo();
  }

  /* Games related */
  BaseGearHandler* GetRawGearHandler() {
    return m_gear_handler.get();
  }
  QStringList GetAvailableGearHandlers() {
    static QStringList out{};
    if (out.empty()) {
      std::for_each_n(GetAvailableGearHandlersNames().cbegin(),
                      GetAvailableGearHandlersNames().size() - 1,
                      [&](const auto& name) { out.append(name); });
    }
    return out;
  }

  /* Input related */
  ControllerHandler* GetRawControllerHandler() {
    return m_controller_handler.get();
  }

  /* Main */
  Q_INVOKABLE void OnMainWindowLoaded();

 private:
  ServiceManager();

  std::unique_ptr<ControllerHandler> m_controller_handler{nullptr};

  std::unique_ptr<BaseGearHandler> m_gear_handler{nullptr};

  btc2::WarningNotification m_tmp{};
};

}  // namespace btc2
