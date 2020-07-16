#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include "enums.hpp"
#include "surface.hpp"
#include "util.hpp"

#include <array>
#include <cstdint>
#include <optional>
#include <span>
#include <string_view>

namespace sdl2 {

/**
 * @brief 
 */ 
class window {
    SDL_Window* window_;

public:
    /**
     * @brief Explicit constructor for the SDL2 C API.
     * @param window The SDL_Window to take ownership of.
     */
    constexpr explicit window(SDL_Window& window) noexcept
        : window_(std::addressof(window)) {}

    /**
     * @brief Copy constructor deleted.
     */
    window(window const&) = delete;

    /**
     * @brief Copy assignment deleted.
     */
    window& operator=(window const&) = delete;

    /**
     * @brief Move assignment deleted.
     */
    window& operator=(window&&) = delete;

    /**
     * @brief Move constructor
     * @param other The window object to move in this window.
     */
    constexpr window(window&& other) noexcept
        : window_(std::exchange(other.window_, nullptr)) {}

    /**
     *
     */
    static constexpr xy<int> pos_centered{SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED};

    /**
     * 
     */
    static constexpr xy<int> pos_undefined{SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED};

    /**
     * @brief Construct an SDL window.
     * @param title The title of the window. 
     * @param xy The position of the window.
     * @param wh The width/height of the window.
     * @param flgs The window flag specifications.
     * @return An optional window if successfully created.
     */
    static std::optional<window> create(null_term_string const title, xy<int> const xy,
                                        wh<int> const wh, window_flags const flgs) noexcept;

    /**
     * @brief Construct a window from another. 
     * @param other The window object to copy data from.
     * @return An optional window if successfully created.
    */
    static std::optional<window> copy(window const& other) noexcept;

    /**
     * @brief The destructor.
     */
    ~window() noexcept;

    /**
     * @brief Get a pointer to the underlying SDL representation.
     * @return A pointer to the underlying SDL_Window.
     */
    constexpr auto native_handle() const noexcept { return window_; }

    /**
     * @brief Destroy the underlying window object.
     * @note Accessing the window after this functional call is UB.
     */
    void destroy() noexcept;

    /**
     * @brief Gets the size of the window's borders.
     * @return An optional borders size if this is supported on the platform.
     */
    std::optional<tlbr<int>> borders_size() noexcept;

    /**
     * @brief Get the brightness (gamma multiplier) of the window.
     * @return The current brightness of the window.
     */
    float brightness() const noexcept;
    
    //SDL_GetWindowData
    
    /**
     * @brief The display index of the window.
     * @return The index or -1 on error.
     */
    int display_index() const noexcept;

    /**
     * @brief Get the display mode to use when a window is visible at fullscreen.
     * @return An optional SDL_DisplayMode if the call succeeded.  
     */
    std::optional<SDL_DisplayMode> display_mode() const noexcept;

    /**
     * @brief Get the window flags used to create this window.
     * @return The window flags enum.
     */
    window_flags flags() const noexcept;

    // SDL_GetWindowFromID

    /**
     * @brief Get the gamma ramp for the display that owns this window.
     * @return rgb<> struct containing the translation tables for thier respective channels.
     */ 
    rgb<std::array<std::uint16_t, 256>> gamma_ramp() const noexcept;

    /**
     * @brief 
     * @return
     */
    bool is_grabbed() const noexcept;

    /**
     * @brief 
     * @return
     */
    std::uint32_t id() const noexcept;

    /**
     * @brief 
     * @return
     */
    wh<int> maximum_size() const noexcept;

    /**
     * @brief 
     * @return
     */
    wh<int> minimum_size() const noexcept;

    /**
     * @brief 
     * @return
     */
    float opacity() const noexcept;

    /**
     * @brief 
     * @return
     */
    pixel_format_enum pixel_format() const noexcept;

    /**
     * @brief 
     * @return
     */
    xy<int> position() const noexcept;

    /**
     * @brief 
     * @return
     */
    wh<int> size() const noexcept;

    // SDL_GetWindowSurface - TODO

    /**
     * @brief 
     * @return
     */
    std::string_view title() const noexcept;

    /**
     * @brief 
     * @return
     */
    std::optional<SDL_SysWMinfo> wm_info() const noexcept;

    /**
     * @brief 
     */
    void hide() noexcept;

    /**
     * @brief 
     */
    void maximize() noexcept;
    
    /**
     * @brief 
     */
    void minimize() noexcept;

    /**
     * @brief 
     */
    void raise() noexcept;

    /**
     * @brief 
     */
    void restore() noexcept;

    /**
     * @brief
     * @param borderer 
     */
    void set_bordered(bool const bordered) noexcept;

    /**
     * @brief
     * @param brightness 
     */
    bool set_brightness(float const brightness) noexcept;

    // SDL_SetWindowData

    /**
     * @brief
     * @param dm
     * @return 
     */
    bool set_display_mode(SDL_DisplayMode const& dm) noexcept;

    /**
     * @brief
     * @return 
     */
    bool set_display_mode() noexcept;

    /**
     * @brief
     * @param flags
     * @return 
     */
    bool set_fullscreen(fullscreen_flags const flags) noexcept;

    /**
     * @brief
     * @param r
     * @param g
     * @param b
     * @return 
     */
    bool set_gamma_ramp(std::span<std::uint16_t const, 256> const r, 
                        std::span<std::uint16_t const, 256> const g, 
                        std::span<std::uint16_t const, 256> const b) noexcept;

    /**
     * @brief
     * @param grabbed
     */
    void set_grabbed(bool const grabbed = true) noexcept;

    /**
     * @brief
     * @param fn
     * @return 
     */
    bool set_hit_test(function_ref<SDL_HitTestResult(window&, SDL_Point const&)> const fn) noexcept;

    /**
     * @brief
     * @param fn
     * @return 
     */
    bool set_hit_test(function_ref<SDL_HitTestResult(window const&, SDL_Point const&)> const fn) noexcept;

    /**
     * @brief
     * @param s
    */
    void set_icon(surface const& s) noexcept;

    /**
     * @brief
     * @return
     */
    bool set_input_focus() noexcept;

    /**
     * @brief
     * @param wh
     */
    void set_maximum_size(wh<int> const wh) noexcept;

    /**
     * @brief
     * @param wh
     */
    void set_minimum_size(wh<int> const wh) noexcept;

    /**
     * @brief
     * @param parent
     * @return
     */
    bool set_modal_for(window const& parent) noexcept;

    /**
     * @brief
     * @param opacity
     * @return
     */
    bool set_opacity(float const opacity) noexcept;

    /**
     * @brief
     * @param xy
     */
    void set_position(xy<int> const xy) noexcept;

    /**
     * @brief
     * @param resizable
     */
    void set_resizable(bool const resizable) noexcept;

    /**
     * @brief
     * @param wh
     */
    void set_size(wh<int> const wh) noexcept;

    /**
     * @brief
     * @param title
     */
    void set_title(null_term_string const title) noexcept;

    /**
     * @brief
     * @param flags
     * @param title
     * @param message
     * @return
     */
    bool show_simple_message_box(message_box_flags const flags, null_term_string const title, null_term_string const message) noexcept;

    /**
     * @brief
     */
    void show() noexcept;

    /**
     * @brief
     */
    bool update_surface() noexcept;

    /**
     * @brief
     * @param rects
     * @return 
     */
    bool update_surface_rects(std::span<SDL_Rect const> const rects) noexcept;
};

} // namespace sdl2