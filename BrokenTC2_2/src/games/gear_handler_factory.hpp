#pragma once

#include <QStringList>

#include <DataStructures/games.hpp>
#include <games/gear_handler_the_crew.hpp>

namespace btc2 {

std::unique_ptr<BaseGearHandler> MakeGearHandler(Game::Types handler_type);

}  // namespace btc2
