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

/**
 * @brief 
 */ 
class texture_lock {
    SDL_Texture* const texture_;
    void* const pixels_;
    int const pitch_;

    friend class texture;

    constexpr texture_lock(SDL_Texture* const t, void* const pix, int const pitch) noexcept
        : texture_(t), pixels_(pix), pitch_(pitch) {}

public:
    /**
     * @brief Copy constructor deleted.
     */ 
    texture_lock(texture_lock const&) = delete;

    /**
     * @brief Copy assignment deleted.
     */ 
    texture_lock& operator=(texture_lock const&) = delete;

    /**
     * @brief Get the pitch of the underlying texture pixel data.
     * @return The pitch value.
     */ 
    constexpr int pitch() const noexcept { return pitch_; }

    /**
     * @brief Get access to the locked texture's pixels.
     * @return Pointer to the pixel data.
     */ 
    constexpr void* pixels() const noexcept { return pixels_; }
    
    /**
     * @brief The destructor.
     */ 
    ~texture_lock() noexcept {
        SDL_UnlockTexture(texture_);
    }
};

/**
 * @brief 
 */ 
class texture {
    SDL_Texture* texture_;
    
public:
    /**
     * @brief
     * @param r
     * @param format
     * @param access
     * @param wh
     * @return
    */
    static std::optional<texture> create(renderer& r, pixel_format_enum const format, texture_access const access, wh<int> const wh) noexcept;

    /**
     * @brief
     * @param r
     * @param s
     * @return
    */
    static std::optional<texture> create(renderer& r, surface const& s) noexcept;

    /**
     * @brief
     * @param r
     * @param file
     * @return
    */
    static std::optional<texture> create(renderer& r, null_term_string file) noexcept;

    /**
     * @brief Constructer for interfacing with SDL's C API.
     * @param t An SDL_Texture struct to take ownership of.
    */
    constexpr texture(SDL_Texture& t) noexcept 
        : texture_(std::addressof(t)) {}

    /**
     * @brief Copy constructor deleted.
    */
    texture(texture const&) = delete;

    /**
     * @brief Copy assignment deleted.
    */
    texture& operator=(texture const&) = delete;

    /**
     * @brief Move assignment deleted.
    */
    texture& operator=(texture&&) = delete;

    /**
     * @brief Move constructor.
     * @param other The texture object to move into this texture.
    */
    constexpr texture(texture&& other) noexcept;

    /**
     * @brief The destructor.
    */
    ~texture() noexcept;

    /**
     * @brief Get a pointer to the underlying SDL representation.
     * @return A pointer to the underlying SDL_Texture.
     */
    constexpr SDL_Texture* native_handle() const noexcept { 
        return texture_; 
    }

    /**
     * @brief Destroy the underlying texture object.
     * @note Accessing the texture after this functional call is UB.
     */
    void destroy() noexcept;
    
    /**
     * @brief 
     * @return
     */
    texture_lock lock() noexcept;

    /**
     * @brief 
     * @param rect
     * @return
     */
    texture_lock lock(rect<int> const& rect) noexcept;

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
    rgb<std::uint8_t> color_mode() const noexcept;

    /**
     * @brief A struct containing texture information.
     */
    struct texture_query { 
        pixel_format_enum format = pixel_format_enum::UNKNOWN;
        texture_access access{};
        int width = 0, height = 0; 
    };

    /**
     * @brief 
     * @return
     */
    texture_query query() const noexcept;

    /**
     * @brief 
     * @return
     */
    wh<int> size() const noexcept;

    /**
     * @brief 
     * @return
     */
    pixel_format_enum format() const noexcept;

    /**
     * @brief 
     * @return
     */
    texture_access access() const noexcept;

    /**
     * @brief 
     * @param alpha
     * @return
     */
    bool set_alpha_mode(std::uint8_t alpha) noexcept;

    /**
     * @brief 
     * @param mode
     * @return
     */
    bool set_blend_mode(sdl2::blend_mode mode) noexcept;

    /**
     * @brief 
     * @param mod
     * @return
     */
    bool set_color_mod(rgb<> const mod) noexcept;

    /**
     * @brief 
     * @param rect
     * @param pixels
     * @param pitch
     * @return
     */
    bool update(rect<int> const& rect, std::span<pixel_color const> pixels, int pitch) noexcept;

    /**
     * @brief
     * @param pixels
     * @param pitch
     * @return
     */
    bool update(std::span<pixel_color const> pixels, int pitch) noexcept;

    /**
     * @brief
     * @param rect
     * @param yplane
     * @param ypitch
     * @param uplane 
     * @param upitch
     * @param vplane
     * @param vpitch
     * @return
     */
    bool update_yuv(rect<int> const& rect, 
                    std::uint8_t const* yplane, int ypitch,
                    std::uint8_t const* uplane, int upitch,
                    std::uint8_t const* vplane, int vpitch) noexcept;

    /**
     * @brief
     * @param yplane
     * @param ypitch
     * @param uplane 
     * @param upitch
     * @param vplane
     * @param vpitch
     * @return
     */
    bool update_yuv(std::uint8_t const* yplane, int ypitch,
                    std::uint8_t const* uplane, int upitch,
                    std::uint8_t const* vplane, int vpitch) noexcept;

};  

} // namespace sdl2