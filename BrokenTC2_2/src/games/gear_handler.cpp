#include "gear_handler.hpp"

#include <QQmlEngine>

#include <Logger/logger.hpp>

namespace btc2 {

using GearType = BaseGearHandler::GearType;
using GearMode = BaseGearHandler::GearMode;

void BaseGearHandler::Init() {
  SPDLOG_INFO("Init");

  qmlRegisterUncreatableType<btc2::BaseGearHandler>("btc2", 1, 0, "BaseGearHandler", "Should only be created from CPP");
}

GearType BaseGearHandler::GetGear() const {
  return m_gear;
}
void BaseGearHandler::SetGear(GearType gear) {
  gear = std::clamp(gear, GetMinGear(), GetMaxGear());
  if (gear == m_gear) {
    return;
  }
  const auto kOldGear{m_gear};
  m_gear = gear;
  OnGearSet(kOldGear, m_gear);
  emit gearChanged();
}

GearMode BaseGearHandler::GetGearMode() const {
  return m_mode;
}
void BaseGearHandler::SetGearMode(GearMode mode) {
  if (mode == m_mode) {
    return;
  }
  const auto kOldMode{m_mode};
  m_mode = mode;
  SPDLOG_INFO("Mode set: {} ({})", GetGearModeStr(), GetGearMode());
  OnGearModeSet(kOldMode, m_mode);
  emit gearModeChanged();
}
void BaseGearHandler::CycleMode() {
  if (m_mode + 1 < GearMode::kMaxEnumValue) {
    SetGearMode(static_cast<GearMode>(m_mode + 1));
  } else {
    SetGearMode(GearMode{});
  }
}

QString BaseGearHandler::GetGearModeStr() {
  return GetGearModeStr(GetGearMode());
}

QString BaseGearHandler::GetGearStr() {
  return GearToString(GetGear());
}

QString GearToString(BaseGearHandler::GearType gear) {
  if (gear == 0) {
    return "N";
  } else if (gear == -1) {
    return "R";
  }
  return QString::number(gear);
}

}  // namespace btc2
