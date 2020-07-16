#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "enums.hpp"

#include <cstdint>
#include <concepts>
#include <optional>
#include <string_view>

namespace sdl2 {

class SDL2 {
    bool valid_ = true;
    constexpr SDL2() noexcept = default;

public:
    SDL2(SDL2 const&) = delete;
    SDL2& operator=(SDL2 const&) = delete;
    SDL2& operator=(SDL2&&) = delete;
    
    constexpr SDL2(SDL2&& other) noexcept = default;

    static std::optional<SDL2> init(sdl2_init_flags const& flgs) noexcept {
        if (SDL_Init(static_cast<std::uint32_t>(flgs)) == 0)
            return SDL2{};
        return {};
    }

    inline static std::string_view get_error() noexcept {
        return SDL_GetError();
    }

    ~SDL2() noexcept { if (valid_) SDL_Quit(); }
};

class IMG {
    bool valid_ = true;
    constexpr IMG() noexcept = default;

public:
    IMG(IMG const&) = delete;
    IMG& operator=(IMG const&) = delete;
    IMG& operator=(IMG&&) = delete;

    constexpr IMG(IMG&& other) noexcept 
        : valid_(std::exchange(other.valid_, false)) {}

    static std::optional<IMG> init(img_init_flags const& flgs) noexcept {
        if (IMG_Init(static_cast<int>(flgs)) == 0)
            return IMG{};
        return {};
    }

    ~IMG() noexcept { if (valid_) IMG_Quit(); }
};
    
} // namespace sdl