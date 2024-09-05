#pragma once

#include <QStringList>

#include <DataStructures/games.hpp>
#include <games/gear_handler_the_crew.hpp>

namespace btc2 {

inline std::unique_ptr<BaseGearHandler> MakeGearHandler(Game handler_type) {
  switch (handler_type) {
    case Game::kTheCrew2:
    case Game::kTheCrewMotorfist:
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
