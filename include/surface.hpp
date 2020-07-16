#pragma once

#include <SDL2/SDL.h>

#include <atomic>
#include <optional>

#include "color.hpp"
#include "shapes.hpp"
#include "util.hpp"

namespace sdl2 {

class surface {
    SDL_Surface* surface_;

public:
    static std::optional<surface> create(wh<int> _wh, int depth, rgba<std::uint32_t> masks) noexcept;
    static std::optional<surface> create(void* pixels, int pitch, wh<int> _wh, int depth, rgba<std::uint32_t> masks) noexcept;
    static std::optional<surface> create(pixel_format_enum fmt, int depth, wh<int> _wh) noexcept;
    static std::optional<surface> create(void* pixels, int pitch, pixel_format_enum fmt, int depth, wh<int> _wh) noexcept;
    static std::optional<surface> create(null_term_string file) noexcept;

    constexpr explicit surface(SDL_Surface& s) noexcept : surface_(std::addressof(s)) {}

    surface(surface const&) = delete;
    surface& operator=(surface const&) = delete;
    surface& operator=(surface&&) = delete;

    constexpr surface(surface&& other) noexcept;

    ~surface() noexcept;

    constexpr auto native_handle() const noexcept { return surface_; }


    constexpr const_pixel_format_view pixel_format() const noexcept;
    
    constexpr int width() const noexcept;
    constexpr int height() const noexcept;
    constexpr int pitch() const noexcept;
    constexpr void const* pixels() const noexcept;
    constexpr void* pixels() noexcept;
    constexpr int num_pixels() const noexcept;
    constexpr void const* userdata() const noexcept;
    constexpr void* userdata() noexcept;
    constexpr void set_userdata(void* const userdata) noexcept;
    constexpr rect<int> clip_rect() const noexcept;
    constexpr int refcount() const noexcept;
    constexpr int refcount_add(int const amt = 1) noexcept;
    constexpr int refcount_sub(int const amt = 1) noexcept;

    int refcount_atomic_load(std::memory_order const order = std::memory_order_seq_cst) const noexcept;
    int refcount_atomic_fetch_add(int const amt = 1, std::memory_order const order = std::memory_order_seq_cst) noexcept;
    int refcount_atomic_fetch_sub(int const amt = 1, std::memory_order const order = std::memory_order_seq_cst) noexcept;

    void lock() noexcept;
    void unlock() noexcept;
    bool must_lock() const noexcept;

    // blit
    bool blit(rect<int> const& srcrect, surface& dst, rect<int>& dstrect) noexcept;
    bool blit(surface& dst, rect<int>& dstrect) noexcept;
    bool blit(surface& dst) noexcept;
    bool blit(rect<int> const& srcrect, surface& dst) noexcept;

    // blit_scaled
    bool blit_scaled(rect<int> const& srcrect, surface& dst, rect<int>& dstrect) noexcept;
    bool blit_scaled(surface& dst, rect<int>& dstrect) noexcept;
    bool blit_scaled(surface& dst) noexcept;
    bool blit_scaled(rect<int> const& srcrect, surface& dst) noexcept;

    // lower_blit
    bool lower_blit(rect<int> const& srcrect, surface& dst, rect<int>& dstrect) noexcept;
    bool lower_blit(surface& dst, rect<int>& dstrect) noexcept;
    bool lower_blit(surface& dst) noexcept;
    bool lower_blit(rect<int> const& srcrect, surface& dst) noexcept;

    // lower_blit_scaled
    bool lower_blit_scaled(rect<int> const& srcrect, surface& dst, rect<int>& dstrect) noexcept;
    bool lower_blit_scaled(surface& dst, rect<int>& dstrect) noexcept;
    bool lower_blit_scaled(surface& dst) noexcept;
    bool lower_blit_scaled(rect<int> const& srcrect, surface& dst) noexcept;

    bool fill_rect(rect<int> const& rect, pixel_color const color) noexcept;
    bool fill(pixel_color const color) noexcept;
    bool fill_rects(std::span<rect<int> const> const rects, pixel_color const color) noexcept;

    bool convert(sdl2::pixel_format const& fmt) noexcept;
    std::optional<surface> convert_to_new(sdl2::pixel_format const& fmt) const noexcept;

    std::optional<pixel_color> color_key() const noexcept;

    std::uint8_t alpha_mod() const noexcept;

    sdl2::blend_mode blend_mode() const noexcept;

    rgb<std::uint8_t> color_mod() const noexcept;

    bool set_clip_rect(rect<int> const& rect) noexcept;

    void disable_clipping() noexcept;

    bool set_color_key(bool const enable, pixel_color const color) noexcept;

    bool set_alpha_mod(std::uint8_t const alpha) noexcept;

    bool set_blend_mode(sdl2::blend_mode const mode) noexcept;

    bool set_color_mode(rgb<std::uint8_t> const _rgb) noexcept;

    bool set_palette(palette_view const p) noexcept;

    bool set_rle(bool const enable) noexcept;

    bool save_bmp_to_file(null_term_string const file) const noexcept;
};

bool convert_pixels(wh<int> _wh, surface const& src, surface& dst) noexcept;

} // namespace sdl2