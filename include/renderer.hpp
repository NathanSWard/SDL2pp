#pragma once

#include <optional>
#include <span>

#include "enums.hpp"
#include "surface.hpp"
#include "texture.hpp"
#include "window.hpp"

namespace sdl2 {

class texture;

/**
 * @brief A struct containing renderer info. 
 */
class renderer_info {
    SDL_RendererInfo info_{};

public:
    /**
     * @brief Get a pointer to the underlying SDL representation.
     * @return A pointer to a SDL_RendererInfo.
     */
    constexpr SDL_RendererInfo const* native_handle() const noexcept { return &info_; }

    /**
     * @brief Get a pointer to the underlying SDL representation.
     * @return A pointer to a SDL_RendererInfo.
     */
    constexpr SDL_RendererInfo* native_handle() noexcept { return &info_; }

    /**
     * @brief Get the name of the renderer.
     * @return The name of the renderer.
     */
    constexpr std::string_view name() const noexcept { return info_.name; }

    /**
     * @brief Get a mask of the renderer's flags.
     * @return The renerer flags enum.
     */
    constexpr renderer_flags flags() const noexcept { return static_cast<renderer_flags>(info_.flags); }

    /**
     * @brief Get the renderer's available texture formats.
     * @return A span of pixel_format_enum's.
     */
    std::span<pixel_format_enum const> texture_formats() const noexcept;

    /**
     * @brief Get the renderer's maximum texture width.
     * @return The maximum texture width.
     */
    constexpr int max_texture_width() const noexcept { return info_.max_texture_width; }

    /**
     * @brief Get the renderer's maximum texture height.
     * @return The maximum texture height.
     */
    constexpr int max_texture_height() const noexcept { return info_.max_texture_height; }
};

/**
 * @brief A wrapper around an SDL_Renderer structure.
 */ 
class renderer {
    SDL_Renderer* renderer_;

public: 
    /**
     * @brief Explicit constructor for the SDL2 C API.
     * @param r The SDL_Renderer to take ownership of.
     */
    constexpr explicit renderer(SDL_Renderer* r) noexcept 
        : renderer_{r} {}

    /**
     * @brief Copy constructor deleted.
     */
    renderer(renderer const&) = delete;

    /**
     * @brief Copy assignment deleted.
     */
    renderer& operator=(renderer const&) = delete;

    /**
     * @brief Move assignment deleted. 
     */
    renderer& operator=(renderer&&) = delete;

    /**
     * @brief Move constructor. 
     */
    constexpr renderer(renderer&& other) noexcept
        : renderer_(std::exchange(other.renderer_, nullptr)) {}

    /**
     * @brief Creates a 2D rendering context for a window.
     * @param win The window where the rendering is displayed.
     * @param flags The renderer flags options.
     * @param device_index Index of driver to initialize, or -1 to initialze first avilable device.
     */
    renderer(window& win, renderer_flags flags, int device_index = -1) noexcept;
    
    /**
     * @brief Creates a 2D software rendering context for a surface.
     * @param s The surface where the rendering is done.
     */
    explicit renderer(surface& s) noexcept;

    /**
     * @brief Destructor. 
     */
    ~renderer() noexcept;

    /**
     * @brief Destroy the underlying renderer object.
     * @note Accessing the renderer after this functional call is UB.
     */
    void destroy() noexcept;

    /**
     * @brief Get a pointer to the underlying SDL representation.
     * @return A pointer to the underlying SDL_Renderer.
     */
    constexpr SDL_Renderer* native_handle() const noexcept { return renderer_; }

    /**
     * @brief Checks if the renderer is in a valid state.
     * @return True if valid, false if not.
     */
    constexpr explicit operator bool() const noexcept { return renderer_ != nullptr; }

    /**
     * @brief Checks if the renderer is in a valid state.
     * @return True if valid, false if not.
     */
    constexpr bool is_ok() const noexcept { return renderer_ != nullptr; }

    /**
     * @brief Get the blend mode used for drawing ops.
     * @return The renderer's blend mode.
     */
    blend_mode draw_blend_mode() const noexcept;

    /**
     * @brief Get the color used for the renderer's drawing operations.
     * @return The rgba representation of the draw color.
     */
    rgba<> draw_color() const noexcept;

    /**
     * @brief Get information about the renderer.
     * @return A renderer_info struct filled with information about the renderer.
     */
    renderer_info info() const noexcept;

    /**
     * @brief Get the output size in pixels of the renderer.
     * @return The output size.
     */
    wh<> output_size() const noexcept;

    /**
     * @brief Get the current renderer's target.
     * @return A SDL_Texture or an empty optional_ref if the default target is used.
     */
    optional_ref<SDL_Texture const> get_target() const noexcept;

    /**
     * @brief Get the current renderer's target.
     * @return A SDL_Texture or an empty optional_ref if the default target is used.
     */
    optional_ref<SDL_Texture> get_target() noexcept;

    /**
     * @brief Clear the rendering target with the drawing color.
     * @return True if succeeded, false if failed.
     */
    bool clear() noexcept;

    /**
     * @brief Copies a portion of a texture to a portion of the rendering target.
     * @param render_rect The area of the renering target to copy to.
     * @param txr The source texture.
     * @param txr_rect The area of the texture to copy.
     * @return True if succeeded, false if failed.
     * @note The texture will be stretched to fill the render_rect.
     * @return True if succeeded, false if failed.
     */
    bool copy(rect<int> const& render_rect, texture const& txr, rect<int> const& txr_rect) noexcept;

    /**
     * @brief Copies of portion of a teture to the entire rendering target.
     * @param txr The source texture.
     * @param txr_rect The area of the texture to copy.
     * @return True if succeeded, false if failed. 
     * @note The texture will be stretched to fill the render target.
     * @return True if succeeded, false if failed.
     */
    bool copy(texture const& txr, rect<int> const& txr_rect) noexcept;

    /**
     * @brief Copies a texture to a portion of the rendering target.
     * @param render_rect The area of the renering target to copy to.
     * @param txr The source texture.
     * @return True if succeeded, false if failed. 
     * @note The texture will be stretched to fill the render_rect.
     * @return True if succeeded, false if failed.
     */
    bool copy(rect<int> const& render_rect, texture const& txr) noexcept;

    /**
     * @brief Copies a texture to the rendering target.
     * @param txr The source texture.
     * @return True if succeeded, false if failed. 
     * @note The texture will be stretched to fill the render target.
     * @return True if succeeded, false if failed.
     */
    bool copy(texture const& txr) noexcept;

    /**
     * @brief Copy a portion of a texture to a portion of the rendering target,
     * rotating it around the given center and flipping it top-btm and/or left-right.
     * @param render_rect The area of the renering target to copy to.
     * @param txr The source texture. 
     * @param txr_rect The area of the texture to copy.
     * @param angle Angle in degrees of the rotation (applied clockwise).
     * @param center The point indicating the point around which render_rect will be rotated.
     * @param flip The flip actions to be performed on the texture.
     * @return True if succeeded, false if failed.
     */
    bool copy_ex(rect<int> const& render_rect, texture const& txr, rect<int> const& txr_rect, double const angle, point<int> const& center, renderer_flip flip = renderer_flip::NONE) noexcept;

    /**
     * @brief Copy a portion of a texture to the rendering target,
     * rotating it around the given center and flipping it top-btm and/or left-right.
     * @param txr The source texture.
     * @param txr_rect The area of the texture to copy.
     * @param angle Angle in degrees of the rotation (applied clockwise).
     * @param center The point indicating the point around which render_rect will be rotated.
     * @param flip The flip actions to be performed on the texture. 
     * @return True if succeeded, false if failed.
     */
    bool copy_ex(texture const& txr, rect<int> const& txr_rect, double angle, point<int> const& center, renderer_flip flip = renderer_flip::NONE) noexcept;

    /**
     * @brief Copy a texture to a portion of the rendering target,
     * rotating it around the given center and flipping it top-btm and/or left-right.
     * @param render_rect The area of the renering target to copy to.
     * @param txr The source texture.
     * @param angle Angle in degrees of the rotation (applied clockwise).
     * @param center The point indicating the point around which render_rect will be rotated.
     * @param flip The flip actions to be performed on the texture. 
     * @return True if succeeded, false if failed.
     */
    bool copy_ex(rect<int> const& render_rect, texture const& txr, double angle, point<int> const& center, renderer_flip flip = renderer_flip::NONE) noexcept;

    /**
     * @brief Copy a texture to the rendering target,
     * rotating it around the given center and flipping it top-btm and/or left-right.
     * @param txr The source texture.
     * @param angle Angle in degrees of the rotation (applied clockwise).
     * @param center The point indicating the point around which render_rect will be rotated.
     * @param flip The flip actions to be performed on the texture. 
     * @return True if succeeded, false if failed.
     */
    bool copy_ex(texture const& txr, double angle, point<int> const& center, renderer_flip flip = renderer_flip::NONE) noexcept;

    /**
     * @brief Copy a portion of a texture to the rendering target,
     * optionally rotating it around the renderer target's center and flipping it top-btm and/or left-right.
     * @param txr The source texture.
     * @param txr_rect The area of the texture to copy.
     * @param angle Angle in degrees of the rotation (applied clockwise).
     * @param flip The flip actions to be performed on the texture. 
     * @return True if succeeded, false if failed.
     */
    bool copy_ex(texture const& txr, rect<int> const& txr_rect, double angle = 0.0, renderer_flip flip = renderer_flip::NONE) noexcept;

    /**
     * @brief Copy a texture to a portion of the rendering target,
     * optionally rotating it around render_rect's center and flipping it top-btm and/or left-right.
     * @param render_rect The area of the renering target to copy to.
     * @param txr The source texture.
     * @param angle Angle in degrees of the rotation (applied clockwise).
     * @param flip The flip actions to be performed on the texture. 
     * @return True if succeeded, false if failed.
     */
    bool copy_ex(rect<int> const& render_rect, texture const& txr, double angle = 0.0, renderer_flip flip = renderer_flip::NONE) noexcept;

    /**
     * @brief Copy a texture to a rendering target,
     * optionally rotating it around the rendering target's center and flipping it top-btm and/or left-right.
     * @param txr The source texture.
     * @param angle Angle in degrees of the rotation (applied clockwise).
     * @param flip The flip actions to be performed on the texture. 
     * @return True if succeeded, false if failed.
     */
    bool copy_ex(texture const& txr, double angle = 0.0, renderer_flip flip = renderer_flip::NONE) noexcept;

    /**
     * @brief Draw a line on the current rendering target.
     * @param from The start point.
     * @param to The end point.
     * @return True if succeeded, false if failed.
     */
    template<class Rep>
    bool draw_line(point<Rep> const& from, point<Rep> const& to) noexcept;

    /**
     * @brief Draw a series of connected lines on the current rendering target.
     * @param points A span of points representing the points along the line.
     * @return True if succeeded, false if failed.
     */
    template<class Rep>
    bool draw_lines(std::span<point<Rep> const> points) noexcept;

    /**
     * @brief Draw a point on the current rendering target.
     * @param p The point to draw.
     * @return True if succeeded, false if failed.
     */
    template<class Rep>
    bool draw_point(point<Rep> const& p) noexcept;

    /**
     * @brief Draw points on the current rendering target.
     * @param points A span of points to draw.
     * @return True if succeeded, false if failed.
     */
    template<class Rep>
    bool draw_points(std::span<point<Rep> const> points) noexcept;

    /**
     * @brief Draw a rectangle on the current rendering target.
     * @param r The rect to draw.
     * @return True if succeeded, false if failed.
     */
    template<class Rep>
    bool draw_rect(rect<Rep> const& r) noexcept;

    /**
     * @brief Draw a rectangle outlining the current rendering target.
     * @return True if succeeded, false if failed.
     */
    bool draw_outline() noexcept;

    /**
     * @brief Draw rectangles on the current rendering target.
     * @param rs A span of rectangles to draw.
     * @return True if succeeded, false if failed.
     */
    template<class Rep>
    bool draw_rects(std::span<rect<Rep> const> rs) noexcept;

    /**
     * @brief Fill a rectangle on the current rendering target.
     * @param r The rectangle to color.
     * @return True if succeeded, false if failed.
     */
    template<class Rep>
    bool fill_rect(rect<Rep> const& r) noexcept;

    /**
     * @brief Fill the current rendering target with the current draw color.
     * @return True if succeeded, false if failed.
    */
    bool fill_target() noexcept;

    /**
     * @brief Fill rectangles on the current rendering target.
     * @param rs A span of rectangles to fill.
     * @return True if succeeded, false if failed.
     */
    template<class Rep>
    bool fill_rects(std::span<rect<Rep> const> rs) noexcept;

    /**
     * @brief Get the clipping rectange for the current target. 
     * @return The clipping area or an empty rectangle if clipping is disabled.
     */
    rect<int> clip_rect() const noexcept;

    /**
     * @brief Get whether integer scales are forced for resolution-independent rendering.
     * @return True if integer scales are forced or false if not.
     */
    bool integer_scale() const noexcept;

    /**
     * @brief Get the device independent resolution for rendering.
     * @return The logical size of the renderer.
     */
    wh<> logical_size() const noexcept;

    /**
     * @brief Get the drawing scale for the current target.
     * @return The horizonal(x) and verticle(y) scaling factors.
     */
    xy<float> scale() const noexcept;

    /**
     * @brief Get the drawing area for the current target.
     * @return The current drawing area.
     */
    rect<int> viewport() const noexcept;

    /**
     * @brief Get whether clipping is enabled on the renderer.
     * @return True if clipping is enabled, false if disabled.
     */
    bool is_clip_enabled() const noexcept;

    /**
     * @brief Update the screen with any rendering performed since the previous call.
     */
    void present() const noexcept;

    /**
     * @brief Copy an area of pixels from the rendering target.
     * @param r The area of pixels to read.
     * @param fmt The format of the pixel data.
     * @param pixels A pointer to the pixel data to copy into.
     * @param pitch The pitch of the pixels.
     * @return True if succeeded, false if failed.
     * @warning This is a slow operation and should not be used frequently.
     */
    bool read_pixels(rect<int> const& r, pixel_format_enum fmt, void* pixels, int pitch) const noexcept;
    
    /**
     * @brief Copy all pixels from the rendering target.
     * @param fmt The format of the pixel data.
     * @param pixels A pointer to the pixel data to copy into.
     * @param pitch The pitch of the pixels.
     * @return True if succeeded, false if failed. 
     * @warning This is a slow operation and should not be used frequently.
     */
    bool read_pixels(pixel_format_enum fmt, void* pixels, int const pitch) const noexcept;
    
    /**
     * @brief Copy an area of pixels from the rendering target using the renderer's pixel format.
     * @param r
     * @param pixels A pointer to the pixel data to copy into.
     * @param pitch The pitch of the pixels.
     * @return True if succeeded, false if failed.
     * @warning This is a slow operation and should not be used frequently. 
     */
    bool read_pixels(rect<int> const& r, void* pixels, int pitch) const noexcept;
    
    /**
     * @brief Copy all pixels from the rendering target using the renderer's pixel format.
     * @param pixels A pointer to the pixel data to copy into.
     * @param  pitch The pitch of the pixels.
     * @return True if succeeded, false if failed. 
     * @warning This is a slow operation and should not be used frequently.
     */
    bool read_pixels(void* pixels, int pitch) const noexcept;

    /**
     * @brief Sets the clip rectangle for rendering on the target.
     * @param clip The clip rectangle.
     * @return True if succeeded, false if failed.
     */
    bool set_clip_rect(rect<int> const& clip) noexcept;

    /**
     * @brief Disables clipping on the rendering target.
     * @return True if succeeded, false if failed. 
     */
    bool disable_clipping() noexcept;

    /**
     * @brief Sets whether to force integer scales from resolution-independent rendering.
     * @param enable A bool specifying if integer scaling should be enabled.
     * @return True if succeeded, false if failed.  
     */
    bool set_integer_scale(bool enable) noexcept;

    /**
     * @brief Sets a device independent resolution for rendering.
     * @param size The size of the logical resolution.
     * @return True if succeeded, false if failed.
     */
    bool set_logical_size(wh<int> const& size) noexcept;

    /**
     * @brief Sets the drawing scale for rendering on the current target.
     * @param scale The horizontal(x) and verticlal(y) scaling factors.
     * @return True if succeeded, false if failed.
     */
    bool set_scale(xy<float> const& scale) noexcept;

    /**
     * @brief Set the renderer's drawing area on the current target.
     * @param r The drawing area.
     * @return True if succeeded, false if failed.
     */
    bool set_viewport(rect<int> const& r) noexcept;

    /**
     * @brief Sets the viewport to the entier rendering target.
     * @return True if succeeded, false if failed.
     */
    bool reset_viewport() noexcept;

    /**
     * @brief Determines wheather a window supports the use of render targets.
     * @param r The renderer to check.
     * @return Ture if supported, false if not.
     */
    static bool target_supported(renderer const& r) noexcept;

    /**
     * @brief Sets the blend mode used for drawing opertaions.
     * @param mode The mode to use for blending.
     * @return True if succeeded, false if failed.
     */
    bool set_draw_blend_mode(blend_mode mode) noexcept;

    /**
     * @brief Sets the renderer's draw color.
     * @param color The new draw color.
     * @return True if succeeded, false if failed.
     */
    bool set_draw_color(rgba<> color) noexcept;

    /**
     * @brief Sets a texture as the renderer's target.
     * @param t The texture to target.
     * @return True if succeeded, false if failed.
     * @note The texture must have been created with texture_access::TARGET.
     */
    bool set_render_target(texture const& t) noexcept;

    /**
     * @brief Resets the render target to the default render target.
     * @return True if succeeded, false if failed.
     */
    bool reset_render_target() noexcept;
};

/**
 * @brief Creates a window and default renderer.
 * @param wh The width/height of the window.
 * @param flags the flags used to create the window.
 * @return An optinal pair of sdl2::window and sdl2::renderer, or any empty optional if the function failed. 
 */
inline std::pair<window, renderer> create_window_and_renderer(wh<int> wh, window_flags flags) noexcept;

// sdl2::renderer template method implementations
template<class Rep>
bool renderer::draw_line(point<Rep> const& from, point<Rep> const& to) noexcept {
    if constexpr (std::is_same_v<Rep, int>)
        return SDL_RenderDrawLine(renderer_, from.x, from.y, to.x, to.y) == 0;
    else
        return SDL_RenderDrawLineF(renderer_, from.x, from.y, to.x, to.y) == 0;
}

template<class Rep>
bool renderer::draw_lines(std::span<point<Rep> const> const points) noexcept {
    if constexpr (std::is_same_v<Rep, int>)
        return SDL_RenderDrawLines(renderer_, points.data()->native_handle(), static_cast<int>(points.size())) == 0;
    else
        return SDL_RenderDrawLinesF(renderer_, points.data()->native_handle(), static_cast<int>(points.size())) == 0;
}

template<class Rep>
bool renderer::draw_point(point<Rep> const& p) noexcept {
    if constexpr (std::is_same_v<Rep, int>)
        return SDL_RenderDrawPoint(renderer_, p.x, p.y) == 0;
    else
        return SDL_RenderDrawPointF(renderer_, p.x, p.y) == 0;
}

template<class Rep>
bool renderer::draw_points(std::span<point<Rep> const> const points) noexcept {
    if constexpr (std::is_same_v<Rep, int>)
        return SDL_RenderDrawPoints(renderer_, points.data()->native_handle(), static_cast<int>(points.size())) == 0;
    else
        return SDL_RenderDrawPointsF(renderer_, points.data()->native_handle(), static_cast<int>(points.size())) == 0;
}

template<class Rep>
bool renderer::draw_rect(rect<Rep> const& r) noexcept {
    if constexpr (std::is_same_v<Rep, int>)
        return SDL_RenderDrawRect(renderer_, r.native_handle()) == 0;
    else
        return SDL_RenderDrawRectF(renderer_, r.native_handle()) == 0;
}

template<class Rep>
bool renderer::draw_rects(std::span<rect<Rep> const> const r) noexcept {
    if constexpr (std::is_same_v<Rep, int>)
        return SDL_RenderDrawRects(renderer_, r.data().native_handle(), static_cast<int>(r.size())) == 0;
    else
        return SDL_RenderDrawRectsF(renderer_, r.data().native_handle(), static_cast<int>(r.size())) == 0;
}

template<class Rep>
bool renderer::fill_rect(rect<Rep> const& r) noexcept {
    if constexpr (std::is_same_v<Rep, int>)
        return SDL_RenderFillRect(renderer_, r.native_handle()) == 0;
    else
        return SDL_RenderFillRectF(renderer_, r.native_handle()) == 0;
}

template<class Rep>
bool renderer::fill_rects(std::span<rect<Rep> const> const r) noexcept {
    if constexpr (std::is_same_v<Rep, int>)
        return SDL_RenderFillRects(renderer_, r.data().native_handle(), static_cast<int>(r.size())) == 0;
    else
        return SDL_RenderFillRectsF(renderer_, r.data().native_handle(), static_cast<int>(r.size())) == 0;
}

} // namespace sdl2

