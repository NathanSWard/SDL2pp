#pragma once

#include <SDL2/SDL.h>

#include <atomic>
#include <optional>

#include "color.hpp"
#include "shapes.hpp"
#include "util.hpp"

namespace sdl2 {

/**
 * @brief 
 */ 
class surface {
    SDL_Surface* surface_;

public:
    /**
     * @brief
     * @param wh
     * @param depth 
     * @param masks
     */
    surface(wh<int> wh, int depth, rgba<std::uint32_t> masks) noexcept;

    /**
     * @brief
     * @param pixels
     * @param pitch
     * @param wh
     * @param depth 
     * @param masks
     */
    surface(void* pixels, int pitch, wh<int> wh, int depth, rgba<std::uint32_t> masks) noexcept;

    /**
     * @brief
     * @param fmt
     * @param depth
     * @param wh
     */
    surface(pixel_format_enum fmt, int depth, wh<int> wh) noexcept;

    /**
     * @brief
     * @param pixels
     * @param pitch
     * @param fmt
     * @param depth
     * @param wh
     */
    surface(void* pixels, int pitch, pixel_format_enum fmt, int depth, wh<int> wh) noexcept;

    /**
     * @brief
     * @param file
     */
    surface(null_term_string file) noexcept;

    /**
     * @brief 
     * @param s 
     */
    constexpr explicit surface(SDL_Surface* s) noexcept 
        : surface_(s) {}

    /**
     * @brief Copy constructor deleted. 
     */
    surface(surface const&) = delete;

    /**
     * @brief Copy assignment deleted. 
     */
    surface& operator=(surface const&) = delete;

    /**
     * @brief Move assignment deleted. 
     */
    surface& operator=(surface&&) = delete;

    /**
     * @brief Move constructor.
     * @param other The surface object to move into this surface.
    */
    constexpr surface(surface&& other) noexcept;

    /**
     * @brief Checks if the surface is in a valid state.
     * @return True if valid, false if not.
     */
    constexpr explicit operator bool() const noexcept { return surface_ != nullptr; }

    /**
     * @brief Checks if the surface is in a valid state.
     * @return True if valid, false if not.
     */
    constexpr bool is_ok() const noexcept { return surface_ != nullptr; }

    /**
     * @brief The destructor. 
     */
    ~surface() noexcept;

    /**
     * @brief Get a pointer to the underlying SDL representation.
     * @return A pointer to the underlying SDL_Surface.
     */
    constexpr auto native_handle() const noexcept { return surface_; }

    /**
     * @brief
     * @return
     */
    constexpr const_pixel_format_view pixel_format() const noexcept;
    
    /**
     * @brief
     * @return
     */
    constexpr int width() const noexcept;

    /**
     * @brief
     * @return
     */
    constexpr int height() const noexcept;

    /**
     * @brief
     * @return
     */
    constexpr int pitch() const noexcept;

    /**
     * @brief
     * @return
     */
    constexpr void const* pixels() const noexcept;

    /**
     * @brief
     * @return
     */
    constexpr void* pixels() noexcept;

    /**
     * @brief
     * @return
     */
    constexpr int num_pixels() const noexcept;

    /**
     * @brief
     * @return
     */
    constexpr void const* userdata() const noexcept;

    /**
     * @brief
     * @return
     */
    constexpr void* userdata() noexcept;

    /**
     * @brief
     * @param userdata
     */
    constexpr void set_userdata(void* userdata) noexcept;

    /**
     * @brief
     * @return
     */
    constexpr rect<int> clip_rect() const noexcept;

    /**
     * @brief
     * @return
     */
    constexpr int refcount() const noexcept;

    /**
     * @brief
     * @param amt
     * @return
     */
    constexpr int refcount_add(int amt = 1) noexcept;

    /**
     * @brief
     * @param amt
     * @return
     */
    constexpr int refcount_sub(int amt = 1) noexcept;

    /**
     * @brief
     * @param order
     * @return
     */
    int refcount_atomic_load(std::memory_order order = std::memory_order_seq_cst) const noexcept;
    
    /**
     * @brief
     * @param amt
     * @param order
     * @return
     */
    int refcount_atomic_fetch_add(int amt = 1, std::memory_order order = std::memory_order_seq_cst) noexcept;
    
    /**
     * @brief
     * @param amt
     * @param order
     * @return
     */
    int refcount_atomic_fetch_sub(int amt = 1, std::memory_order order = std::memory_order_seq_cst) noexcept;

    /**
     * @brief 
     */
    void lock() noexcept;

    /**
     * @brief 
     */
    void unlock() noexcept;

    /**
     * @brief 
     * @return
     */
    bool must_lock() const noexcept;

    /**
     * @brief
     * @param srcrect
     * @param dst
     * @param dstrect
     * @return True if succeeded, false if failed.
     */
    bool blit(rect<int> const& srcrect, surface& dst, rect<int>& dstrect) noexcept;

    /**
     * @brief
     * @param dst
     * @param dstrect
     * @return True if succeeded, false if failed.
     */
    bool blit(surface& dst, rect<int>& dstrect) noexcept;

    /**
     * @brief
     * @param dst
     * @return True if succeeded, false if failed.
     */
    bool blit(surface& dst) noexcept;

    /**
     * @brief
     * @param srcrect
     * @param dst
     * @return True if succeeded, false if failed.
     */
    bool blit(rect<int> const& srcrect, surface& dst) noexcept;

    /**
     * @brief
     * @param srcrect
     * @param dst
     * @param dstrect
     * @return True if succeeded, false if failed.
     */
    bool blit_scaled(rect<int> const& srcrect, surface& dst, rect<int>& dstrect) noexcept;

    /**
     * @brief
     * @param dst
     * @param dstrect
     * @return True if succeeded, false if failed.
     */
    bool blit_scaled(surface& dst, rect<int>& dstrect) noexcept;

    /**
     * @brief
     * @param dst
     * @return True if succeeded, false if failed.
     */
    bool blit_scaled(surface& dst) noexcept;

    /**
     * @brief
     * @param srcrect
     * @param dst
     * @return True if succeeded, false if failed.
     */
    bool blit_scaled(rect<int> const& srcrect, surface& dst) noexcept;

    /**
     * @brief
     * @param srcrect
     * @param dst
     * @param dstrect
     * @return True if succeeded, false if failed.
     */
    bool lower_blit(rect<int> const& srcrect, surface& dst, rect<int>& dstrect) noexcept;

    /**
     * @brief
     * @param dst
     * @param dstrect
     * @return True if succeeded, false if failed.
     */
    bool lower_blit(surface& dst, rect<int>& dstrect) noexcept;

    /**
     * @brief
     * @param dst
     * @return True if succeeded, false if failed.
     */
    bool lower_blit(surface& dst) noexcept;

    /**
     * @brief
     * @param srcrect
     * @param dst
     * @return True if succeeded, false if failed.
     */
    bool lower_blit(rect<int> const& srcrect, surface& dst) noexcept;

    /**
     * @brief
     * @param srcrect
     * @param dst
     * @param dstrect
     * @return True if succeeded, false if failed.
     */
    bool lower_blit_scaled(rect<int> const& srcrect, surface& dst, rect<int>& dstrect) noexcept;

    /**
     * @brief
     * @param dst
     * @param dstrect
     * @return True if succeeded, false if failed.
     */
    bool lower_blit_scaled(surface& dst, rect<int>& dstrect) noexcept;

    /**
     * @brief
     * @param dst
     * @return True if succeeded, false if failed.
     */
    bool lower_blit_scaled(surface& dst) noexcept;

    /**
     * @brief
     * @param srcrect
     * @param dst
     * @return True if succeeded, false if failed.
     */
    bool lower_blit_scaled(rect<int> const& srcrect, surface& dst) noexcept;

    /**
     * @brief
     * @param rect
     * @param color 
     * @return True if succeeded, false if failed.
     */
    bool fill_rect(rect<int> const& rect, pixel_value color) noexcept;

    /**
     * @brief
     * @param color 
     * @return True if succeeded, false if failed.
     */
    bool fill(pixel_value color) noexcept;

    /**
     * @brief
     * @param rects
     * @param color 
     * @return True if succeeded, false if failed.
     */
    bool fill_rects(std::span<rect<int> const> rects, pixel_value color) noexcept;

    /**
     * @brief
     * @param fmt
     * @return True if succeeded, false if failed.
     */
    bool convert(sdl2::pixel_format const& fmt) noexcept;

    /**
     * @brief
     * @param fmt
     * @return
     */
    surface convert_to_new(sdl2::pixel_format const& fmt) const noexcept;

    /**
     * @brief
     * @return 
     */
    std::optional<pixel_value> color_key() const noexcept;

    /**
     * @brief
     * @return 
     */
    std::uint8_t alpha_mod() const noexcept;

    /**
     * @brief
     * @return 
     */
    sdl2::blend_mode blend_mode() const noexcept;

    /**
     * @brief
     * @return 
     */
    rgb<std::uint8_t> color_mod() const noexcept;

    /**
     * @brief
     * @return True if succeeded, false if failed. 
     */
    bool set_clip_rect(rect<int> const& rect) noexcept;

    /**
     * @brief
     * @return True if succeeded, false if failed.
     */
    bool disable_clipping() noexcept;

    /**
     * @brief
     * @param color
     * @return True if succeeded, false if failed. 
     */
    bool set_color_key(bool enable, pixel_value color) noexcept;

    /**
     * @brief
     * @param alpha
     * @return True if succeeded, false if failed. 
     */
    bool set_alpha_mod(std::uint8_t const alpha) noexcept;

    /**
     * @brief
     * @param mode
     * @return True if succeeded, false if failed.  
     */
    bool set_blend_mode(sdl2::blend_mode mode) noexcept;

    /**
     * @brief
     * @param rgb
     * @return True if succeeded, false if failed.  
     */
    bool set_color_mode(rgb<std::uint8_t> rgb) noexcept;

    /**
     * @brief
     * @param p
     * @return True if succeeded, false if failed.  
     */
    bool set_palette(palette_view p) noexcept;

    /**
     * @brief
     * @param enable
     * @return True if succeeded, false if failed.  
     */
    bool set_rle(bool enable) noexcept;

    /**
     * @brief
     * @param file
     * @return True if succeeded, false if failed.  
     */
    bool save_bmp_to_file(null_term_string file) const noexcept;
};

/**
 * @brief
 * @param wh 
 * @param src
 * @param dst
 * @return True if succeeded, false if failed. 
 */
bool convert_pixels(wh<int> wh, surface const& src, surface& dst) noexcept;

} // namespace sdl2