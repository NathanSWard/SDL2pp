#pragma once

#include <SDL2/SDL.h>

#include <concepts>
#include <cstdint>
#include <optional>
#include <span>

#include "enums.hpp"
#include "util.hpp"

namespace sdl2 {

namespace colors {
    inline constexpr auto red = rgba<>{255, 0, 0, 255};
    inline constexpr auto orange = rgba<>{255, 165, 0, 255};
    inline constexpr auto yellow = rgba<>{255, 255, 0, 255};
    inline constexpr auto green = rgba<>{0, 128, 0, 255};
    inline constexpr auto blue = rgba<>{0, 0, 255, 255};
    inline constexpr auto purple = rgba<>{128, 0, 128, 255};
    inline constexpr auto black = rgba<>{0, 0, 0, 255};
    inline constexpr auto white = rgba<>{255, 255, 255, 255};
}

} // namespace sdl2