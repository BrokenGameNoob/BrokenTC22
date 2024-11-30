#include "gear_handler.hpp"

#include <QQmlEngine>

#include <Logger/logger.hpp>
#include <utils/shared_constants.hpp>

namespace btc2 {

using GearType = BaseGearHandler::GearType;
using GearMode = GearHandlerMode::Type;

void BaseGearHandler::Init() {
  SPDLOG_INFO("Init");

  qmlRegisterUncreatableType<btc2::BaseGearHandler>("btc2", 1, 0, "BaseGearHandler", "Should only be created from CPP");
}

GearType BaseGearHandler::GetGear() const {
  return m_gear;
}
void BaseGearHandler::SetGear(GearType gear) {
  if (!IsActive()) {
    return;
  }

  gear = std::clamp(gear, GetMinGear(), GetMaxGear());
  const auto kOldGear{m_gear};
  m_gear = gear;
  OnGearSet(kOldGear, m_gear);
  emit gearChanged();
}

GearHandlerMode::Type BaseGearHandler::GetGearMode() const {
  return m_mode;
}
void BaseGearHandler::SetGearMode(GearHandlerMode::Type mode) {
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
  if (m_mode + 1 < GearMode::MAX_MODE_ENUM_VALUE) {
    SetGearMode(static_cast<GearMode>(m_mode + 1));
  } else {
    SetGearMode(GearMode{});
  }
}

QString BaseGearHandler::GetGearModeStr() {
  if (!IsActive()) {
    return tr("Disabled");
  }
  return GetGearModeStr(GetGearMode());
}

QString BaseGearHandler::GetGearStr() {
  return GearToString(GetGear());
}

QString BaseGearHandler::GetGearModeIconSource() {
  switch (GetGearMode()) {
    case GearMode::CLUTCH_MODE:
      return Constants::kIconFast;
      break;
    case GearMode::SEQ_MODE:
      return Constants::kIconSlow;
    default:
      SPDLOG_ERROR("Failed to find an icon for gear mode: {}", GetGearModeStr());
      break;
  }
  return Constants::kIconCancel;
}

void BaseGearHandler::GearUp() {
  if (!IsActive()) {
    return;
  }
  InternalGearUp();
}

void BaseGearHandler::GearDown() {
  if (!IsActive()) {
    return;
  }
  InternalGearDown();
}

bool BaseGearHandler::IsActive() const {
  return IsUserEnabled() && IsSoftEnabled();
}

void BaseGearHandler::SetUserEnabled(bool enabled) {
  const bool kOldActive{IsActive()};
  m_user_enabled = enabled;
  if (kOldActive != IsActive()) {
    emit activeChanged();
  }
}

bool BaseGearHandler::IsUserEnabled() const {
  return m_user_enabled;
}

void BaseGearHandler::SetSoftEnabled(bool enabled) {
  const bool kOldActive{IsActive()};
  m_soft_enabled = enabled;
  if (kOldActive != IsActive()) {
    emit activeChanged();
  }
}

bool BaseGearHandler::IsSoftEnabled() const {
  return m_soft_enabled;
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
