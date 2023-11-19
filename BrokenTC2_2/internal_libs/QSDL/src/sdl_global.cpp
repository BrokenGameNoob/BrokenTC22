/* Broken The Crew 2 sequential clutch assist
 * Copyright (C) 2022 BrokenGameNoob <brokengamenoob@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Logger/logger.hpp>
#include <QSDL/sdl_global.hpp>
#include <SDL2/SDL.h>

namespace qsdl {

bool InitSDL(uint64_t flags) {
  if (SDL_Init(flags) < 0) {
    SPDLOG_ERROR("Cannot init SDL: {}", SDL_GetError());
    return false;
  }
  return true;
}

QStringList GetPluggedJoysticks() {
  auto devicesCnt{GetPluggedJoysticksCount()};
  QStringList out{};
  out.reserve(devicesCnt);

  for (int i{}; i < devicesCnt; ++i) {
    out.append(SDL_JoystickNameForIndex(i));
  }
  return out;
}

int FindJoystickByName(const QString name) {
  auto joyList{GetPluggedJoysticks()};
  int i{};
  for (const auto& curName : joyList) {
    if (name == curName) return i;
    ++i;
  }
  return -1;
}

}  // namespace qsdl
