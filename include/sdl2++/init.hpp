#pragma once

#include "SDL.h"
#include "SDL_image.h"

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
    */
    SDL2(sdl2_init_flags const& flgs) noexcept
        : valid_{SDL_Init(static_cast<std::uint32_t>(flgs)) == 0}
    {}

    /**
     * @brief Destructor that deinitializes SDL libraries.
    */
    ~SDL2() noexcept { if (valid_) SDL_Quit(); }

    /**
     * @brief Checks if the SDL libraries were initialized correctly.
     * @return True if successful, false if not.
     */
    constexpr explicit operator bool() const noexcept { return valid_; }

    /**
     * @brief Checks if the SDL libraries were initialized correctly.
     * @return True if successful, false if not.
     */
    constexpr bool is_ok() const noexcept { return valid_; }

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
    */
    IMG(img_init_flags const& flgs) noexcept 
        : valid_{IMG_Init(static_cast<int>(flgs)) == 0}
    {}

    /**
     * @brief Destructor that deinitializes SDL_image libraries.
    */
    ~IMG() noexcept { if (valid_) IMG_Quit(); }

    /**
     * @brief Checks if the SDL_image libraries were initialized correctly.
     * @return True if successful, false if not.
     */
    constexpr explicit operator bool() const noexcept { return valid_; }

    /**
     * @brief Checks if the SDL_image libraries were initialized correctly.
     * @return True if successful, false if not.
     */
    constexpr bool is_ok() const noexcept { return valid_; }

    /**
     * @brief Get the previously set error from within SDL_Image.
     * @return A string denoting the error.
    */
    static std::string_view get_error() noexcept {
        return IMG_GetError();
    }

};
    
} // namespace sdl2