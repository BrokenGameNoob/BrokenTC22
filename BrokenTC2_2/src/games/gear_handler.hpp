#pragma once

#include <QObject>
#include <QVariant>
#include <cstdint>
#include <qqmlintegration.h>

namespace btc2 {

class BaseGearHandler : public QObject {
  Q_OBJECT

  Q_PROPERTY(int gear READ GetGear WRITE SetGear NOTIFY gearChanged FINAL)
  Q_PROPERTY(QString gearStr READ GetGearStr NOTIFY gearChanged FINAL)
  Q_PROPERTY(GearMode gearMode READ GetGearMode WRITE SetGearMode NOTIFY gearModeChanged FINAL)
  Q_PROPERTY(QString gearModeStr READ GetGearModeStr NOTIFY gearModeChanged FINAL)

 public:
  using GearType = int32_t;

  enum GearMode : int32_t { CLUTCH_MODE, SEQ_MODE, kMaxEnumValue };
  Q_ENUM(GearMode)

  static void Init();

 signals:
  void gearChanged();
  void gearModeChanged();

 public:
  BaseGearHandler(QObject* parent) : QObject{parent} {}

  GearType GetGear() const;
  void SetGear(GearType gear);

  GearMode GetGearMode() const;
  void SetGearMode(GearMode mode);
  Q_INVOKABLE void CycleMode();

  Q_INVOKABLE QString GetGearModeStr();
  Q_INVOKABLE QString GetGearStr();

  /* To reimplement */

  Q_INVOKABLE virtual GearType GetMinGear() const = 0;
  Q_INVOKABLE virtual GearType GetMaxGear() const = 0;

  Q_INVOKABLE virtual void GearUp() = 0;
  Q_INVOKABLE virtual void GearDown() = 0;

 protected:
  virtual void OnGearSet(GearType old_gear, GearType gear) {}
  virtual void OnGearModeSet(GearMode old_mode, GearMode mode) {}

 private:
  GearType m_gear{};
  GearMode m_mode{};

 private:
  static constexpr auto GetGearModeStr(const GearMode mode) {
    switch (mode) {
      case CLUTCH_MODE:
        return "Clutch";
      case SEQ_MODE:
        return "Classic";
      default:
        break;
    }
    return "UNKNOWN_MODE";
  }
};

inline std::ostream& operator<<(std::ostream& stream, const BaseGearHandler::GearMode& mode) {
  return stream << QVariant{mode}.toString().toStdString();
}

QString GearToString(BaseGearHandler::GearType gear);

}  // namespace btc2
