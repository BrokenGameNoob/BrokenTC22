#include "gear_handler_factory.hpp"

#include <QQmlEngine>

#include <Logger/logger.hpp>
#include <system/services/service_manager.hpp>

namespace btc2 {

GearHandlerUniquePtr MakeGearHandler(Game::Types handler_type) {
  GearHandlerUniquePtr handler;
  switch (handler_type) {
    case Game::THE_CREW_2:
      SPDLOG_DEBUG("Making The Crew 2 gear handler.");
      handler = GearHandlerUniquePtr{
          new GearHandlerTheCrew{ServiceManager::GetGameProfilesHandler().GetTheCrew2Profile(), nullptr}};
      break;
    case Game::THE_CREW_MOTORFIST:
      SPDLOG_DEBUG("Making The Crew Motorfist gear handler.");
      handler = GearHandlerUniquePtr{
          new GearHandlerTheCrew{ServiceManager::GetGameProfilesHandler().GetTheCrewMotorfistProfile(), nullptr}};
      break;
    default:
      SPDLOG_ERROR("Unknown gear handler: {}", static_cast<int32_t>(handler_type));
      return nullptr;
  }

  auto* tts{ServiceManager::I().GetRawTextToSpeechManager()};
  auto* rawHandler{handler.get()};
  QObject::connect(rawHandler, &BaseGearHandler::gearChanged, rawHandler, [tts, rawHandler]() {
    tts->Speak(QObject::tr("%0").arg(rawHandler->GetGearStr()));
  });
  QObject::connect(rawHandler, &BaseGearHandler::gearModeChanged, rawHandler, [tts, rawHandler]() {
    if (rawHandler->IsActive()) {
      tts->Speak(rawHandler->GetGearModeStr());
    }
  });
  QObject::connect(rawHandler, &BaseGearHandler::activeChanged, rawHandler, [tts, rawHandler]() {
    tts->Speak(rawHandler->IsActive() ? QObject::tr("Enabled") : QObject::tr("Disabled"));
  });

  return handler;
}

}  // namespace btc2
