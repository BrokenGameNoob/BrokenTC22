#include "gear_handler_factory.hpp"

#include <QQmlEngine>

#include <Logger/logger.hpp>
#include <system/services/service_manager.hpp>

namespace btc2 {

std::unique_ptr<BaseGearHandler> MakeGearHandler(Game::Types handler_type) {
  switch (handler_type) {
    case Game::THE_CREW_2:
      SPDLOG_DEBUG("Making The Crew 2 gear handler.");
      return std::make_unique<GearHandlerTheCrew>(ServiceManager::GetGameProfilesHandler().GetTheCrew2Profile(),
                                                  nullptr);
    case Game::THE_CREW_MOTORFIST:
      SPDLOG_DEBUG("Making The Crew Motorfist gear handler.");
      return std::make_unique<GearHandlerTheCrew>(ServiceManager::GetGameProfilesHandler().GetTheCrewMotorfistProfile(),
                                                  nullptr);
    default:
      SPDLOG_ERROR("Unknown gear handler: {}", static_cast<int32_t>(handler_type));
      break;
  }
  return nullptr;
}

}  // namespace btc2
