#pragma once

#include <QObject>
#include <QVariant>
#include <cstdint>
#include <qqmlintegration.h>

namespace btc2 {

class GearHandlerMode : public QObject {
  Q_OBJECT
 public:
  enum Type {
    CLUTCH_MODE,
    SEQ_MODE,
    MAX_MODE_ENUM_VALUE,
  };
  Q_ENUM(Type)
};

class BaseGearHandler : public QObject {
  Q_OBJECT

  Q_PROPERTY(int gear READ GetGear WRITE SetGear NOTIFY gearChanged FINAL)
  Q_PROPERTY(QString gearStr READ GetGearStr NOTIFY gearChanged FINAL)
  Q_PROPERTY(GearHandlerMode::Type gearMode READ GetGearMode WRITE SetGearMode NOTIFY gearModeChanged FINAL)
  Q_PROPERTY(QString gearModeStr READ GetGearModeStr NOTIFY gearModeChanged FINAL)
  Q_PROPERTY(QString gearModeIconSource READ GetGearModeIconSource NOTIFY gearModeChanged FINAL)
  Q_PROPERTY(bool isActive READ IsActive NOTIFY activeChanged FINAL)

 public:
  using GearType = int32_t;

  static void Init();

 signals:
  void gearChanged();
  void gearModeChanged();
  void activeChanged();

 public:
  BaseGearHandler(QObject* parent) : QObject{parent} {
    connect(this, &BaseGearHandler::activeChanged, this, &BaseGearHandler::gearModeChanged);
  };

  GearType GetGear() const;
  void SetGear(GearType gear);

  GearHandlerMode::Type GetGearMode() const;
  void SetGearMode(GearHandlerMode::Type mode);
  Q_INVOKABLE void CycleMode();

  Q_INVOKABLE QString GetGearModeStr();
  Q_INVOKABLE QString GetGearStr();
  Q_INVOKABLE QString GetGearModeIconSource();

  Q_INVOKABLE void GearUp();
  Q_INVOKABLE void GearDown();

  bool IsActive() const;
  void SetUserEnabled(bool enabled);
  bool IsUserEnabled() const;
  void SetSoftEnabled(bool enabled);
  bool IsSoftEnabled() const;

  /* To reimplement */

  Q_INVOKABLE virtual GearType GetMinGear() const = 0;
  Q_INVOKABLE virtual GearType GetMaxGear() const = 0;
  Q_INVOKABLE virtual GearType GetMaxGearClutch() const = 0;

 protected:
  virtual void OnGearSet(GearType old_gear, GearType gear) {}
  virtual void OnGearModeSet(GearHandlerMode::Type old_mode, GearHandlerMode::Type mode) {}

  virtual void InternalGearUp() = 0;
  virtual void InternalGearDown() = 0;

 private:
  GearType m_gear{};
  GearHandlerMode::Type m_mode{};

  bool m_user_enabled{true};
  bool m_soft_enabled{false};

  bool m_use_seq_after_clutch{false};

 private:
  static constexpr auto GetGearModeStr(const GearHandlerMode::Type mode) {
    switch (mode) {
      case GearHandlerMode::CLUTCH_MODE:
        return "Clutch";
      case GearHandlerMode::SEQ_MODE:
        return "Classic";
      default:
        break;
    }
    return "UNKNOWN_MODE";
  }
};

inline std::ostream& operator<<(std::ostream& stream, const GearHandlerMode::Type& mode) {
  return stream << QVariant{mode}.toString().toStdString();
}

QString GearToString(BaseGearHandler::GearType gear);

}  // namespace btc2
