#pragma once

#include "SDL.h"
#include "SDL_syswm.h"

#include "enums.hpp"
#include "shapes.hpp"
#include "surface.hpp"
#include "util.hpp"

#include <array>
#include <cstdint>
#include <optional>
#include <span>
#include <string_view>

namespace sdl2 {

/**
 * @brief A wrapper around an SDL_Window structure.
 */ 
class window {
    SDL_Window* window_;

public:
    /**
     * @brief Explicit constructor for the SDL2 C API.
     * @param window The SDL_Window to take ownership of.
     */
    constexpr explicit window(SDL_Window* window) noexcept
        : window_(window) {}

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
     * @brief Construct a window.
     * @param title The title of the window. 
     * @param xy The position of the window.
     * @param wh The width/height of the window.
     * @param flgs The window flag specifications.
     */
    window(null_term_string title, xy<int> xy, wh<int> wh, window_flags flgs) noexcept;

    /**
     * @brief The destructor.
     */
    ~window() noexcept;

    /**
     * @brief Destroy the underlying window object.
     * @note Accessing the window after this functional call is UB.
     */
    void destroy() noexcept;

    /**
     * @brief Get a pointer to the underlying SDL representation.
     * @return A pointer to the underlying SDL_Window.
     */
    constexpr auto native_handle() const noexcept { return window_; }

    /**
     * @brief Checks if the window is in a valid state.
     * @return True if valid, false if not.
     */
    constexpr explicit operator bool() const noexcept { return window_ != nullptr; }

    /**
     * @brief Checks if the window is in a valid state.
     * @return True if valid, false if not.
     */
    constexpr bool is_ok() const noexcept { return window_ != nullptr; }

    /**
     * @brief An xy position denoting a centered window.
     */
    static constexpr xy<int> pos_centered{SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED};

    /**
     * @brief An xy position denoting an undefined window position.
     */
    static constexpr xy<int> pos_undefined{SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED};

    /**
     * @brief Construct a window from another. 
     * @param other The window object to copy data from.
     * @return An optional window if successfully created.
     * @note This copy operation has an explicit name to avoid unnecessary copying due to a copy constructor call.
    */
    static window copy(window const& other) noexcept;

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
     * @brief Check if this window is grabbed.
     * @return A bool denoting if this window is grabbed.
     */
    bool is_grabbed() const noexcept;

    /**
     * @brief Get this window's id.
     * @return The window's id.
     */
    std::uint32_t id() const noexcept;

    /**
     * @brief Get the maximum size of the window.
     * @return The maximum size of the window.
     */
    wh<int> maximum_size() const noexcept;

    /**
     * @brief Get the minimum size of the window.
     * @return The minimum size of the window.
     */
    wh<int> minimum_size() const noexcept;

    /**
     * @brief Get the opacticy of the window.
     * @return The opacity of the window.
     */
    float opacity() const noexcept;

    /**
     * @brief The the pixel format of the window.
     * @return the pixel format of the window.
     */
    pixel_format_enum pixel_format() const noexcept;

    /**
     * @brief The the current position of the window.
     * @return The x/y cords of the window.
     */
    xy<int> position() const noexcept;

    /**
     * @brief Get the current size of the window.
     * @return The width and height of the window.
     */
    wh<int> size() const noexcept;

    // SDL_GetWindowSurface - TODO

    /**
     * @brief Get the current title of the window.
     * @return The title of the window.
     */
    std::string_view title() const noexcept;

    /**
     * @brief Get driver specific information about the window.
     * @return A SLD_SysWMinfo struct or an empty optional if this operation is not supported.
     */
    std::optional<SDL_SysWMinfo> wm_info() const noexcept;

    /**
     * @brief Hide the window.
     */
    void hide() noexcept;

    /**
     * @brief Maximize the size of the window.
     */
    void maximize() noexcept;
    
    /**
     * @brief Minimize the size of the window.
     */
    void minimize() noexcept;

    /**
     * @brief Raise this window above other windows and set the input focus.
     */
    void raise() noexcept;

    /**
     * @brief Restores the size and position of a minimized/maximized window.
     */
    void restore() noexcept;

    /**
     * @brief Change the border state of a window.
     * @param borderer A bool denoting if the window should be bordered.
     */
    void set_bordered(bool const bordered) noexcept;

    /**
     * @brief Change the brightness of a window.
     * @param brightness The new brightness values for the window.
     */
    bool set_brightness(float const brightness) noexcept;

    // SDL_SetWindowData

    /**
     * @brief Sets the display mode to use when the window is at fullscreen.
     * @param dm The SDL_DisplayMode strut denoting the window's new display mode.
     * @return True if succeeded, false if failed.
     */
    bool set_display_mode(SDL_DisplayMode const& dm) noexcept;

    /**
     * @brief Set the window's display mode to use the current dimensions and the desktop refresh rate.
     * @return True if succeeded, false if failed.
     */
    bool set_display_mode() noexcept;

    /**
     * @brief Set the window's fullscreen property.
     * @param flags The fullscreen enum type.
     * @return True if succeeded, false if failed. 
     */
    bool set_fullscreen(fullscreen_flags const flags) noexcept;

    /**
     * @brief Set the gamma ramp for the display that owns this window.
     * @param r The translation table for the red channel.
     * @param g The translation table for the green channel.
     * @param b The translation table for the blue channel.
     * @return True if succeeded, false if failed. 
     */
    bool set_gamma_ramp(std::span<std::uint16_t const, 256> const r, 
                        std::span<std::uint16_t const, 256> const g, 
                        std::span<std::uint16_t const, 256> const b) noexcept;

    /**
     * @brief Change the grabbed state of the window.
     * @param grabbed The bool denoting if the window should be grabbed.
     */
    void set_grabbed(bool grabbed = true) noexcept;

    /**
     * @brief Provides a callback that decides if a window region has special properties.
     * @param fn A callable triggered when doing a hit-test.
     * @return True if succeeded, false if failed. 
     * @note Function must have the signature `SDL_HitTestResult(window&, point<int>)`    
     */
    bool set_hit_test(function_ref<SDL_HitTestResult(window&, point<int>)> const fn) noexcept;

    /**
     * @brief Provides a callback that decides if a window region has special properties.
     * @param fn A callable triggered when doing a hit-test.
     * @return True if succeeded, false if failed. 
     * @note Function must have the signature `SDL_HitTestResult(window const&, point<int>)`
     */
    bool set_hit_test(function_ref<SDL_HitTestResult(window const&, point<int>)> const fn) noexcept;

    /**
     * @brief Set the icon for a window.
     * @param s A surface to use as the icon.
    */
    void set_icon(surface const& s) noexcept;

    /**
     * @brief Explicitly set input focus to this window.
     * @return True if succeeded, false if failed.
     */
    bool set_input_focus() noexcept;

    /**
     * @brief Set the maximum size of the window.
     * @param wh The maximum size.
     */
    void set_maximum_size(wh<int> wh) noexcept;

    /**
     * @brief Set the minimum size of the window.
     * @param wh The minimum size.
     */
    void set_minimum_size(wh<int> const wh) noexcept;

    /**
     * @brief Set this window as a modal for another window.
     * @param parent The parent window for this window.
     * @return True if succeeded, false if failed.
     */
    bool set_modal_for(window const& parent) noexcept;

    /**
     * @brief Set the opacity of the window.
     * @param opacity The new opacity value.
     * @return True if succeeded, false if failed.
     * @note The provided opacity is clamped between (0.f, 1.f)
     */
    bool set_opacity(float const opacity) noexcept;

    /**
     * @brief Set the position of the window.
     * @param xy The new position.
     */
    void set_position(xy<int> const xy) noexcept;

    /**
     * @brief Set if the window is resizable.
     * @param resizable A bool denoting if the window is resizable.
     */
    void set_resizable(bool const resizable) noexcept;

    /**
     * @brief Set the current size of the window.
     * @param wh The new size.
     */
    void set_size(wh<int> const wh) noexcept;

    /**
     * @brief Set the title of the window.
     * @param title The window's new title.
     */
    void set_title(null_term_string const title) noexcept;

    /**
     * @brief Displays a simple modal message box.
     * @param flags The mesage box enum flags.
     * @param title Message box's title.
     * @param message Message box's message.
     * @return True if succeeded, false if failed.
     */
    bool show_simple_message_box(message_box_flags const flags, null_term_string const title, null_term_string const message) noexcept;

    /**
     * @brief Shows the window.
     */
    void show() noexcept;

    /**
     * @brief Copies the dinow surface to the screen.
     * @return True if succeeded, false if failed.
     */
    bool update_surface() noexcept;

    /**
     * @brief Copy areas of the window surface to the screen. 
     * @param rects The areas of the surface to copy.
     * @return True if succeeded, false if failed. 
     */
    bool update_surface_rects(std::span<rect<int> const> const rects) noexcept;
};

} // namespace sdl2