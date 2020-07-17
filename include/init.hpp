#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "enums.hpp"

#include <optional>
#include <string_view>

namespace sdl2 {

/**
 * @brief Initialization context for SDL
*/
class SDL2 {
    bool valid_ = true;
    constexpr SDL2() noexcept = default;

public:
    SDL2(SDL2 const&) = delete;
    SDL2& operator=(SDL2 const&) = delete;
    SDL2& operator=(SDL2&&) = delete;
    
    constexpr SDL2(SDL2&& other) noexcept = default;

    /**
     * @brief Initialize the internal SDL libraries.
     * @param flgs Initialization flags.
     * @return An optional SDL2 context struct if initialization succeded.
    */
    static std::optional<SDL2> init(sdl2_init_flags const& flgs) noexcept {
        if (SDL_Init(static_cast<std::uint32_t>(flgs)) == 0)
            return SDL2{};
        return {};
    }

    /**
     * @brief Destructor that deinitializes SDL libraries.
    */
    ~SDL2() noexcept { if (valid_) SDL_Quit(); }

    /**
     * @brief Get the previously set error fro, within SDL.
     * @return A string denoting the error.
    */
    static std::string_view get_error() noexcept {
        return SDL_GetError();
    }
};

/**
 * @brief Initialization context for SDL_Image
*/
class IMG {
    bool valid_ = true;
    constexpr IMG() noexcept = default;

public:
    IMG(IMG const&) = delete;
    IMG& operator=(IMG const&) = delete;
    IMG& operator=(IMG&&) = delete;

    constexpr IMG(IMG&& other) noexcept 
        : valid_(std::exchange(other.valid_, false)) {}

    /**
     * @brief Initialize the internal SDL_Image libraries.
     * @param flgs Initialization flags.
     * @return An optional IMG context struct if initialization succeded.
    */
    static std::optional<IMG> init(img_init_flags const& flgs) noexcept {
        if (IMG_Init(static_cast<int>(flgs)) == 0)
            return IMG{};
        return {};
    }

    /**
     * @brief Destructor that deinitializes SDL_image libraries.
    */
    ~IMG() noexcept { if (valid_) IMG_Quit(); }

    /**
     * @brief Get the previously set error from within SDL_Image.
     * @return A string denoting the error.
    */
    static std::string_view get_error() noexcept {
        return IMG_GetError();
    }

};
    
} // namespace sdl2