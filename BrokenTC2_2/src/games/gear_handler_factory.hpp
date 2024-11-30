#pragma once

#include <QStringList>

#include <DataStructures/games.hpp>
#include <games/gear_handler_the_crew.hpp>

namespace btc2 {

class BaseGearHandlerDeleter {
 public:
  void operator()(BaseGearHandler* handler) const {
    handler->deleteLater();
  }
};

using GearHandlerUniquePtr = std::unique_ptr<BaseGearHandler, BaseGearHandlerDeleter>;

GearHandlerUniquePtr MakeGearHandler(Game::Types handler_type);

}  // namespace btc2
