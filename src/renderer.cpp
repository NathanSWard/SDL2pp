#include "../include/renderer.hpp"

using namespace sdl2;

std::span<pixel_format_enum const> renderer_info::texture_formats() const noexcept { 
    return std::span{reinterpret_cast<pixel_format_enum const*>(info_.texture_formats), info_.num_texture_formats}; 
}

renderer::renderer(window& win, renderer_flags const flags, int const device_index) noexcept\
    : renderer_{SDL_CreateRenderer(win.native_handle(), device_index, static_cast<std::uint32_t>(flags))}
{}

renderer::renderer(surface& s) noexcept
    : renderer_{SDL_CreateSoftwareRenderer(s.native_handle())}
{}

renderer::~renderer() noexcept {
    if (renderer_)
        SDL_DestroyRenderer(renderer_);
}

void renderer::destroy() noexcept {
    SDL_DestroyRenderer(renderer_);
    renderer_ = nullptr;
}

blend_mode renderer::draw_blend_mode() const noexcept {
    SDL_BlendMode bm{};
    [[maybe_unused]] auto const err = SDL_GetRenderDrawBlendMode(renderer_, &bm);
    SDL2_ASSERT(err == 0); 
    return static_cast<blend_mode>(bm);
}

rgba<> renderer::draw_color() const noexcept {
    rgba<> val;
    [[maybe_unused]] auto const err = SDL_GetRenderDrawColor(renderer_, &val.r, &val.g, &val.b, &val.a);
    SDL2_ASSERT(err == 0);
    return val;
}

renderer_info renderer::info() const noexcept {
    renderer_info info{};
    [[maybe_unused]] auto const err = SDL_GetRendererInfo(renderer_, info.native_handle());
    SDL2_ASSERT(err == 0);
    return info;
}

wh<> renderer::output_size() const noexcept {
    wh<> val;
    [[maybe_unused]] auto const err = SDL_GetRendererOutputSize(renderer_, &val.width, &val.height);
    SDL2_ASSERT(err == 0);
    return val;
}

optional_ref<SDL_Texture const> renderer::get_target() const noexcept {
    if (auto const t = SDL_GetRenderTarget(renderer_); t != nullptr)
        return *t;
    return {};
}

optional_ref<SDL_Texture> renderer::get_target() noexcept {
    if (auto const t = SDL_GetRenderTarget(renderer_); t != nullptr)
        return *t;
    return {};
}

bool renderer::draw_outline() noexcept {
    return SDL_RenderDrawRect(renderer_, nullptr) == 0;
}

bool renderer::fill_target() noexcept {
    return SDL_RenderFillRect(renderer_, nullptr) == 0;
}

bool renderer::clear() noexcept {
    return SDL_RenderClear(renderer_) == 0;
}

bool renderer::copy(rect<int> const& render_rect, texture const& txr, rect<int> const& txr_rect) noexcept {
    return SDL_RenderCopy(renderer_, txr.native_handle(), txr_rect.native_handle(), render_rect.native_handle()) == 0;
}
bool renderer::copy(texture const& txr, rect<int> const& txr_rect) noexcept {
    return SDL_RenderCopy(renderer_, txr.native_handle(), txr_rect.native_handle(), nullptr) == 0;
}
bool renderer::copy(rect<int> const& render_rect, texture const& txr) noexcept {
    return SDL_RenderCopy(renderer_, txr.native_handle(), nullptr, render_rect.native_handle()) == 0;
}
bool renderer::copy(texture const& txr) noexcept {
    return SDL_RenderCopy(renderer_, txr.native_handle(), nullptr, nullptr) == 0;
}

bool renderer::copy_ex(rect<int> const& render_rect, texture const& txr, rect<int> const& txr_rect, double const angle, point<int> const& center, renderer_flip const flip) noexcept {
    return SDL_RenderCopyEx(renderer_, txr.native_handle(), txr_rect.native_handle(), render_rect.native_handle(), angle, center.native_handle(), static_cast<SDL_RendererFlip>(flip)) == 0;
}
bool renderer::copy_ex(texture const& txr, rect<int> const& txr_rect, double const angle, point<int> const& center, renderer_flip const flip) noexcept {
    return SDL_RenderCopyEx(renderer_, txr.native_handle(), txr_rect.native_handle(), nullptr, angle, center.native_handle(), static_cast<SDL_RendererFlip>(flip)) == 0;
}
bool renderer::copy_ex(rect<int> const& render_rect, texture const& txr, double const angle, point<int> const& center, renderer_flip const flip) noexcept {
    return SDL_RenderCopyEx(renderer_, txr.native_handle(), nullptr, render_rect.native_handle(), angle, center.native_handle(), static_cast<SDL_RendererFlip>(flip)) == 0;
}
bool renderer::copy_ex(texture const& txr, double const angle, point<int> const& center, renderer_flip const flip) noexcept {
    return SDL_RenderCopyEx(renderer_, txr.native_handle(), nullptr, nullptr, angle, center.native_handle(), static_cast<SDL_RendererFlip>(flip)) == 0;
}
bool renderer::copy_ex(texture const& txr, rect<int> const& txr_rect, double const angle, renderer_flip const flip) noexcept {
    return SDL_RenderCopyEx(renderer_, txr.native_handle(), txr_rect.native_handle(), nullptr, angle, nullptr, static_cast<SDL_RendererFlip>(flip)) == 0;
}
bool renderer::copy_ex(rect<int> const& render_rect, texture const& txr, double const angle, renderer_flip const flip) noexcept {
    return SDL_RenderCopyEx(renderer_, txr.native_handle(), nullptr, render_rect.native_handle(), angle, nullptr, static_cast<SDL_RendererFlip>(flip)) == 0;
}
bool renderer::copy_ex(texture const& txr, double const angle, renderer_flip const flip) noexcept {
    return SDL_RenderCopyEx(renderer_, txr.native_handle(), nullptr, nullptr, angle, nullptr, static_cast<SDL_RendererFlip>(flip)) == 0;
}

rect<int> renderer::clip_rect() const noexcept {
    rect<int> r;
    SDL_RenderGetClipRect(renderer_, r.native_handle());
    return r;
}

bool renderer::integer_scale() const noexcept {
    return SDL_RenderGetIntegerScale(renderer_) == SDL_TRUE;
}

wh<> renderer::logical_size() const noexcept {
    wh<> r;
    SDL_RenderGetLogicalSize(renderer_, &r.width, &r.height);
    return r;
}

xy<float> renderer::scale() const noexcept {
    xy<float> r;
    SDL_RenderGetScale(renderer_, &r.x, &r.y);
    return r;
}

rect<int> renderer::viewport() const noexcept {
    rect<int> r;
    SDL_RenderGetViewport(renderer_, r.native_handle());
    return r;
}

bool renderer::is_clip_enabled() const noexcept {
    return SDL_RenderIsClipEnabled(renderer_) == SDL_TRUE;
}

void renderer::present() const noexcept {
    SDL_RenderPresent(renderer_);
}

bool renderer::read_pixels(rect<int> const& r, pixel_format_enum const fmt, void* const pixels, int const pitch) const noexcept {
    return SDL_RenderReadPixels(renderer_, r.native_handle(), static_cast<std::uint32_t>(fmt), pixels, pitch) == 0;
}
bool renderer::read_pixels(pixel_format_enum const fmt, void* const pixels, int const pitch) const noexcept {
    return SDL_RenderReadPixels(renderer_, nullptr, static_cast<std::uint32_t>(fmt), pixels, pitch) == 0;
}
bool renderer::read_pixels(rect<int> const& r, void* const pixels, int const pitch) const noexcept {
    return SDL_RenderReadPixels(renderer_, r.native_handle(), 0, pixels, pitch) == 0;
}
bool renderer::read_pixels(void* const pixels, int const pitch) const noexcept {
    return SDL_RenderReadPixels(renderer_, nullptr, 0, pixels, pitch) == 0;
}

bool renderer::set_clip_rect(rect<int> const& clip) noexcept {
    return SDL_RenderSetClipRect(renderer_, clip.native_handle()) == 0;
}
bool renderer::disable_clipping() noexcept {
    return SDL_RenderSetClipRect(renderer_, nullptr) == 0;
}

bool renderer::set_integer_scale(bool const enabled) noexcept {
    return SDL_RenderSetIntegerScale(renderer_, static_cast<SDL_bool>(enabled)) == 0;
}

bool renderer::set_logical_size(wh<int> const& size) noexcept {
    return SDL_RenderSetLogicalSize(renderer_, size.width, size.height) == 0;
}

bool renderer::set_scale(xy<float> const& scale) noexcept {
    return SDL_RenderSetScale(renderer_, scale.x, scale.y) == 0;
}

bool renderer::set_viewport(rect<int> const& r) noexcept {
    return SDL_RenderSetViewport(renderer_, r.native_handle()) == 0;
}
bool renderer::reset_viewport() noexcept {
    return SDL_RenderSetViewport(renderer_, nullptr) == 0;
}

bool renderer::target_supported(renderer const& r) noexcept {
    return SDL_RenderTargetSupported(r.native_handle()) == SDL_TRUE;
}

bool renderer::set_draw_blend_mode(blend_mode const mode) noexcept {
    return SDL_SetRenderDrawBlendMode(renderer_, static_cast<SDL_BlendMode>(mode)) == 0;
}

bool renderer::set_draw_color(rgba<> const color) noexcept {
    return SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a) == 0;
}

bool renderer::set_render_target(texture const& t) noexcept {
    SDL2_ASSERT(t.access() == texture_access::TARGET);
    return SDL_SetRenderTarget(renderer_, t.native_handle()) == 0;
}
bool renderer::reset_render_target() noexcept {
    return SDL_SetRenderTarget(renderer_, nullptr) == 0;
}

inline std::pair<window, renderer> create_window_and_renderer(wh<int> const _wh, window_flags const flags) noexcept {
    SDL_Window* w{nullptr};
    SDL_Renderer* r{nullptr};
    SDL_CreateWindowAndRenderer(_wh.width, _wh.height, static_cast<std::uint32_t>(flags), &w, &r);
    return std::pair<window, renderer>(std::piecewise_construct, std::forward_as_tuple(w), std::forward_as_tuple(r));
}