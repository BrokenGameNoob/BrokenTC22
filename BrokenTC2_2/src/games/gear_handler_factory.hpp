#pragma once

#include <QStringList>

#include <games/gear_handler_the_crew.hpp>
#include <utils/macro_define.hpp>

namespace btc2 {

DECLARE_CUSTOM_ENUM(AvailableGearHandlers, kTheCrew2, kTheCrewMotorfest);
DECLARE_ENUM_DISPLAY_NAMES(AvailableGearHandlers, "The Crew 2", "The Crew Motorfist");

inline std::unique_ptr<BaseGearHandler> MakeGearHandler(AvailableGearHandlers handler_type) {
  switch (handler_type) {
    case AvailableGearHandlers::kTheCrew2:
    case AvailableGearHandlers::kTheCrewMotorfest:
      return std::make_unique<GearHandlerTheCrew>(nullptr);
      break;
    default:
      SPDLOG_ERROR("Unknown gear handler: {}", static_cast<int32_t>(handler_type));
      break;
  }
  return nullptr;
}

inline std::unique_ptr<BaseGearHandler> MakeGearHandler(QString handler_type_str) {
  return nullptr;
}

}  // namespace btc2
