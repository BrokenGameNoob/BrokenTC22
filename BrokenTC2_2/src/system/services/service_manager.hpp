#pragma once

#include <QObject>
#include <QQmlEngine>
#include <git_version.hpp>

#include <games/gear_handler_factory.hpp>
#include <games/gear_handler_the_crew.hpp>
#include <system/logs/btype.hpp>

namespace btc2 {

class ServiceManager : public QObject {
  Q_OBJECT
  QML_SINGLETON

  Q_PROPERTY(bool hasDebInfo READ HasDebInfo CONSTANT FINAL)
  Q_PROPERTY(QString versionStr READ GetVersionStr CONSTANT FINAL)
  Q_PROPERTY(QStringList availableGearHandlers READ GetAvailableGearHandlers CONSTANT FINAL)
  Q_PROPERTY(BaseGearHandler* gearHandler READ GetRawGearHandler NOTIFY gearHandlerChanged)

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

 private:
  ServiceManager();

  std::unique_ptr<BaseGearHandler> m_gear_handler{nullptr};
};

}  // namespace btc2
