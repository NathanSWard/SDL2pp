#pragma once

#include <optional>
#include <span>

#include "enums.hpp"
#include "surface.hpp"
#include "texture.hpp"
#include "window.hpp"

namespace sdl2 {

class texture;

class renderer_info {
    SDL_RendererInfo info_{};
public:
    constexpr SDL_RendererInfo const* native_handle() const noexcept { return &info_; }
    constexpr SDL_RendererInfo* native_handle() noexcept { return &info_; }
    constexpr std::string_view name() const noexcept { return info_.name; }
    constexpr renderer_flags flags() const noexcept { return static_cast<renderer_flags>(info_.flags); }
    constexpr std::span<std::uint32_t const> texture_formats() const noexcept { 
        return std::span{info_.texture_formats, info_.num_texture_formats}; 
    }
    constexpr int max_texture_width() const noexcept { return info_.max_texture_width; }
    constexpr int max_texture_height() const noexcept { return info_.max_texture_height; }
};

class renderer {
    SDL_Renderer* renderer_;

public: 
    constexpr SDL_Renderer* native_handle() const noexcept { return renderer_; }

    static std::optional<renderer> create(window& win, renderer_flags const flags, int device_index = -1) noexcept {
        if (auto const r = SDL_CreateRenderer(win.native_handle(), device_index, static_cast<std::uint32_t>(flags)); r != nullptr)
            return renderer{*r};
        return {};
    }
    
    static std::optional<renderer> create_software(surface& s) noexcept {
        if (auto const r = SDL_CreateSoftwareRenderer(s.native_handle()); r != nullptr)
            return renderer{*r};
        return {};
    }

    constexpr explicit renderer(SDL_Renderer& r) noexcept : renderer_(std::addressof(r)) {}

    renderer(renderer const&) = delete;
    renderer& operator=(renderer const&) = delete;
    renderer& operator=(renderer&&) = delete;

    constexpr renderer(renderer&& other) noexcept
        : renderer_(std::exchange(other.renderer_, nullptr)) {}
    
    ~renderer() noexcept {
        if (renderer_)
            SDL_DestroyRenderer(renderer_);
    }

    void destroy() noexcept {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }

    blend_mode draw_blend_mode() const noexcept {
        SDL_BlendMode bm{};
        [[maybe_unused]] auto const err = SDL_GetRenderDrawBlendMode(renderer_, &bm);
        SDL2_ASSERT(err == 0); 
        return static_cast<blend_mode>(bm);
    }

    rgba<> draw_color() const noexcept {
        rgba<> val;
        [[maybe_unused]] auto const err = SDL_GetRenderDrawColor(renderer_, &val.r, &val.g, &val.b, &val.a);
        SDL2_ASSERT(err == 0);
        return val;
    }

    renderer_info info() const noexcept {
        renderer_info info{};
        [[maybe_unused]] auto const err = SDL_GetRendererInfo(renderer_, info.native_handle());
        SDL2_ASSERT(err == 0);
        return info;
    }

    wh<> output_size() const noexcept {
        wh<> val;
        [[maybe_unused]] auto const err = SDL_GetRendererOutputSize(renderer_, &val.width, &val.height);
        SDL2_ASSERT(err == 0);
        return val;
    }

    optional_ref<SDL_Texture const> get_target() const noexcept {
        if (auto const t = SDL_GetRenderTarget(renderer_); t != nullptr)
            return *t;
        return {};
    }

    optional_ref<SDL_Texture> get_target() noexcept {
        if (auto const t = SDL_GetRenderTarget(renderer_); t != nullptr)
            return *t;
        return {};
    }

    bool clear() noexcept {
        return SDL_RenderClear(renderer_) == 0;
    }

    bool copy(rect<int> const& render_rect, texture const& txr, rect<int> const& txr_rect) noexcept {
        return SDL_RenderCopy(renderer_, txr.native_handle(), txr_rect.native_handle(), render_rect.native_handle()) == 0;
    }
    bool copy(texture const& txr, rect<int> const& txr_rect) noexcept {
        return SDL_RenderCopy(renderer_, txr.native_handle(), txr_rect.native_handle(), nullptr) == 0;
    }
    bool copy(rect<int> const& render_rect, texture const& txr) noexcept {
        return SDL_RenderCopy(renderer_, txr.native_handle(), nullptr, render_rect.native_handle()) == 0;
    }
    bool copy(texture const& txr) noexcept {
        return SDL_RenderCopy(renderer_, txr.native_handle(), nullptr, nullptr) == 0;
    }

    bool copy_ex(rect<int> const& render_rect, texture const& txr, rect<int> const& txr_rect, double const angle, point<int> const& center, renderer_flip const flip = renderer_flip::NONE) noexcept {
        return SDL_RenderCopyEx(renderer_, txr.native_handle(), txr_rect.native_handle(), render_rect.native_handle(), angle, center.native_handle(), static_cast<SDL_RendererFlip>(flip)) == 0;
    }
    bool copy_ex(texture const& txr, rect<int> const& txr_rect, double const angle, point<int> const& center, renderer_flip const flip = renderer_flip::NONE) noexcept {
        return SDL_RenderCopyEx(renderer_, txr.native_handle(), txr_rect.native_handle(), nullptr, angle, center.native_handle(), static_cast<SDL_RendererFlip>(flip)) == 0;
    }
    bool copy_ex(rect<int> const& render_rect, texture const& txr, double const angle, point<int> const& center, renderer_flip const flip = renderer_flip::NONE) noexcept {
        return SDL_RenderCopyEx(renderer_, txr.native_handle(), nullptr, render_rect.native_handle(), angle, center.native_handle(), static_cast<SDL_RendererFlip>(flip)) == 0;
    }
    bool copy_ex(texture const& txr, double const angle, point<int> const& center, renderer_flip const flip = renderer_flip::NONE) noexcept {
        return SDL_RenderCopyEx(renderer_, txr.native_handle(), nullptr, nullptr, angle, center.native_handle(), static_cast<SDL_RendererFlip>(flip)) == 0;
    }
    bool copy_ex(texture const& txr, rect<int> const& txr_rect, double const angle, renderer_flip const flip = renderer_flip::NONE) noexcept {
        return SDL_RenderCopyEx(renderer_, txr.native_handle(), txr_rect.native_handle(), nullptr, angle, nullptr, static_cast<SDL_RendererFlip>(flip)) == 0;
    }
    bool copy_ex(rect<int> const& render_rect, texture const& txr, double const angle, renderer_flip const flip = renderer_flip::NONE) noexcept {
        return SDL_RenderCopyEx(renderer_, txr.native_handle(), nullptr, render_rect.native_handle(), angle, nullptr, static_cast<SDL_RendererFlip>(flip)) == 0;
    }
    bool copy_ex(texture const& txr, double const angle, renderer_flip const flip = renderer_flip::NONE) noexcept {
        return SDL_RenderCopyEx(renderer_, txr.native_handle(), nullptr, nullptr, angle, nullptr, static_cast<SDL_RendererFlip>(flip)) == 0;
    }

    template<class Rep>
    bool draw_line(point<Rep> const& from, point<Rep> const& to) noexcept {
        if constexpr (std::is_same_v<Rep, int>)
            return SDL_RenderDrawLine(renderer_, from.x, from.y, to.x, to.y) == 0;
        else
            return SDL_RenderDrawLineF(renderer_, from.x, from.y, to.x, to.y) == 0;
    }

    template<class Rep>
    bool draw_lines(std::span<point<Rep> const> const points) noexcept {
        if constexpr (std::is_same_v<Rep, int>)
            return SDL_RenderDrawLines(renderer_, points.data()->native_handle(), static_cast<int>(points.size())) == 0;
        else
            return SDL_RenderDrawLinesF(renderer_, points.data()->native_handle(), static_cast<int>(points.size())) == 0;
    }

    template<class Rep>
    bool draw_point(point<Rep> const& p) noexcept {
        if constexpr (std::is_same_v<Rep, int>)
            return SDL_RenderDrawPoint(renderer_, p.x, p.y) == 0;
        else
            return SDL_RenderDrawPointF(renderer_, p.x, p.y) == 0;
    }

    template<class Rep>
    bool draw_points(std::span<point<Rep> const> const points) noexcept {
        if constexpr (std::is_same_v<Rep, int>)
            return SDL_RenderDrawPoints(renderer_, points.data()->native_handle(), static_cast<int>(points.size())) == 0;
        else
            return SDL_RenderDrawPointsF(renderer_, points.data()->native_handle(), static_cast<int>(points.size())) == 0;
    }

    template<class Rep>
    bool draw_rect(rect<Rep> const& r) noexcept {
        if constexpr (std::is_same_v<Rep, int>)
            return SDL_RenderDrawRect(renderer_, r.native_handle()) == 0;
        else
            return SDL_RenderDrawRectF(renderer_, r.native_handle()) == 0;
    }

    template<class Rep>
    bool draw_rects(std::span<rect<Rep> const> const r) noexcept {
        if constexpr (std::is_same_v<Rep, int>)
            return SDL_RenderDrawRects(renderer_, r.data().native_handle(), static_cast<int>(r.size())) == 0;
        else
            return SDL_RenderDrawRectsF(renderer_, r.data().native_handle(), static_cast<int>(r.size())) == 0;
    }

    template<class Rep>
    bool fill_rect(rect<Rep> const& r) noexcept {
        if constexpr (std::is_same_v<Rep, int>)
            return SDL_RenderFillRect(renderer_, r.native_handle()) == 0;
        else
            return SDL_RenderFillRectF(renderer_, r.native_handle()) == 0;
    }

    template<class Rep>
    bool fill_rects(std::span<rect<Rep> const> const r) noexcept {
        if constexpr (std::is_same_v<Rep, int>)
            return SDL_RenderFillRects(renderer_, r.data().native_handle(), static_cast<int>(r.size())) == 0;
        else
            return SDL_RenderFillRectsF(renderer_, r.data().native_handle(), static_cast<int>(r.size())) == 0;
    }

    rect<int> clip_rect() const noexcept {
        rect<int> r;
        SDL_RenderGetClipRect(renderer_, r.native_handle());
        return r;
    }

    bool integer_scale() const noexcept {
        return SDL_RenderGetIntegerScale(renderer_) == SDL_TRUE;
    }

    wh<> logical_size() const noexcept {
        wh<> r;
        SDL_RenderGetLogicalSize(renderer_, &r.width, &r.height);
        return r;
    }

    xy<float> scale() const noexcept {
        xy<float> r;
        SDL_RenderGetScale(renderer_, &r.x, &r.y);
        return r;
    }

    rect<int> viewport() const noexcept {
        rect<int> r;
        SDL_RenderGetViewport(renderer_, r.native_handle());
        return r;
    }

    bool is_clip_enabled() const noexcept {
        return SDL_RenderIsClipEnabled(renderer_) == SDL_TRUE;
    }

    void present() const noexcept {
        SDL_RenderPresent(renderer_);
    }

    bool read_pixels(rect<int> const& r, pixel_format_enum const fmt, void* const pixels, int const pitch) const noexcept {
        return SDL_RenderReadPixels(renderer_, r.native_handle(), static_cast<std::uint32_t>(fmt), pixels, pitch) == 0;
    }
    bool read_pixels(pixel_format_enum const fmt, void* const pixels, int const pitch) const noexcept {
        return SDL_RenderReadPixels(renderer_, nullptr, static_cast<std::uint32_t>(fmt), pixels, pitch) == 0;
    }
    bool read_pixels(rect<int> const& r, void* const pixels, int const pitch) const noexcept {
        return SDL_RenderReadPixels(renderer_, r.native_handle(), 0, pixels, pitch) == 0;
    }
    bool read_pixels(void* const pixels, int const pitch) const noexcept {
        return SDL_RenderReadPixels(renderer_, nullptr, 0, pixels, pitch) == 0;
    }

    bool set_clip_rect(rect<int> const& clip) noexcept {
        return SDL_RenderSetClipRect(renderer_, clip.native_handle()) == 0;
    }
    bool disable_clipping() noexcept {
        return SDL_RenderSetClipRect(renderer_, nullptr) == 0;
    }
    
    bool set_integer_scale(bool const enabled) noexcept {
        return SDL_RenderSetIntegerScale(renderer_, static_cast<SDL_bool>(enabled)) == 0;
    }

    bool set_logical_size(wh<int> const& size) noexcept {
        return SDL_RenderSetLogicalSize(renderer_, size.width, size.height) == 0;
    }

    bool set_scale(xy<float> const& scale) noexcept {
        return SDL_RenderSetScale(renderer_, scale.x, scale.y) == 0;
    }

    bool set_viewport(rect<int> const& r) noexcept {
        return SDL_RenderSetViewport(renderer_, r.native_handle()) == 0;
    }
    bool reset_viewport() noexcept {
        return SDL_RenderSetViewport(renderer_, nullptr) == 0;
    }

    static bool target_supported(renderer const& r) noexcept {
        return SDL_RenderTargetSupported(r.native_handle()) == SDL_TRUE;
    }

    bool set_draw_blend_mode(blend_mode const mode) noexcept {
        return SDL_SetRenderDrawBlendMode(renderer_, static_cast<SDL_BlendMode>(mode)) == 0;
    }

    bool set_draw_color(rgba<> const color) noexcept {
        return SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a) == 0;
    }

    bool set_render_target(texture const& t) noexcept {
        SDL2_ASSERT(t.access() == texture_access::TARGET);
        return SDL_SetRenderTarget(renderer_, t.native_handle()) == 0;
    }
    bool reset_render_target() noexcept {
        return SDL_SetRenderTarget(renderer_, nullptr) == 0;
    }
};

inline std::optional<std::pair<window, renderer>> create_window_and_renderer(wh<int> const _wh, window_flags const flags) noexcept {
    SDL_Window* w{nullptr};
    SDL_Renderer* r{nullptr};
    if (SDL_CreateWindowAndRenderer(_wh.width, _wh.height, static_cast<std::uint32_t>(flags), &w, &r) == 0)
        return std::pair<window, renderer>(std::piecewise_construct, std::forward_as_tuple(*w), std::forward_as_tuple(*r));
    if (w) SDL_DestroyWindow(w);
    if (r) SDL_DestroyRenderer(r);
    return {};
}

}