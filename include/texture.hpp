#pragma once

#include <SDL2/SDL.h>

#include "enums.hpp"
#include "util.hpp"
#include "shapes.hpp"
#include "surface.hpp"

#include <optional>

namespace sdl2 {

class renderer;
class texture;

class texture_lock {
    friend class texture;
    SDL_Texture* const texture_;
    void* const pixels_;
    int const pitch_;
    constexpr texture_lock(SDL_Texture* const t, void* const pix, int const pitch) noexcept
        : texture_(t), pixels_(pix), pitch_(pitch) {}

public:
    texture_lock(texture_lock const&) = delete;
    texture_lock& operator=(texture_lock const&) = delete;
    
    constexpr int pitch() const noexcept { return pitch_; }
    constexpr void* pixels() const noexcept { return pixels_; }
    
    ~texture_lock() noexcept {
        SDL_UnlockTexture(texture_);
    }
};

class texture {
    SDL_Texture* texture_;
    
public:
    static std::optional<texture> create(renderer& r, pixel_format_enum const format, texture_access const access, wh<int> const wh) noexcept;
    static std::optional<texture> create(renderer& r, surface const& s) noexcept;
    static std::optional<texture> create(renderer& r, null_term_string file) noexcept;

    constexpr texture(SDL_Texture* const t) noexcept 
        : texture_(t) {}

    texture(texture const&) = delete;
    texture& operator=(texture const&) = delete;
    texture& operator=(texture&&) = delete;
    constexpr texture(texture&& other) noexcept;

    ~texture() noexcept;

    constexpr SDL_Texture* native_handle() const noexcept { return texture_; }

    void destroy() noexcept;

    texture_lock lock() noexcept;
    texture_lock lock(rect<int> const& rect) noexcept;

    std::uint8_t alpha_mod() const noexcept;

    sdl2::blend_mode blend_mode() const noexcept;

    rgb<std::uint8_t> color_mode() const noexcept;

    struct texture_query { 
        pixel_format_enum format = pixel_format_enum::UNKNOWN;
        texture_access access{};
        int width = 0, height = 0; 
    };
    texture_query query() const noexcept;

    wh<int> size() const noexcept;

    pixel_format_enum format() const noexcept;

    texture_access access() const noexcept;

    bool set_alpha_mode(std::uint8_t const alpha) noexcept;

    bool set_blend_mode(sdl2::blend_mode const mode) noexcept;

    bool set_color_mod(rgb<> const mod) noexcept;

    bool update(rect<int> const& rect, std::span<pixel_color const> const pixels, int const pitch) noexcept;

    bool update(std::span<pixel_color const> const pixels, int const pitch) noexcept;

    bool update_yuv(rect<int> const& rect, 
                    std::uint8_t const* yplane, int const ypitch,
                    std::uint8_t const* uplane, int const upitch,
                    std::uint8_t const* vplane, int const vpitch) noexcept;

    bool update_yuv(std::uint8_t const* yplane, int const ypitch,
                    std::uint8_t const* uplane, int const upitch,
                    std::uint8_t const* vplane, int const vpitch) noexcept;

};  

} // namespace sdl2