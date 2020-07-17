#include "../include/surface.hpp"

#include <SDL2/SDL_image.h>

using namespace sdl2;

std::optional<surface> surface::create(wh<int> const _wh, int const depth, rgba<std::uint32_t> const masks) noexcept {
    if (auto const s = SDL_CreateRGBSurface(0, _wh.width, _wh.height, depth, masks.r, masks.g, masks.b, masks.a); s != nullptr)
        return surface{*s};
    return{};
}

std::optional<surface> surface::create(void* const pixels, int const pitch, wh<int> const _wh, 
                                                int const depth, rgba<std::uint32_t> const masks) noexcept {
    if (auto const s = SDL_CreateRGBSurfaceFrom(pixels, _wh.width, _wh.height, depth, pitch, masks.r, masks.g, masks.b, masks.a); s != nullptr)
        return surface{*s};
    return {};
}

std::optional<surface> surface::create(pixel_format_enum const fmt, int const depth, wh<int> const _wh) noexcept {
    if (auto const s = SDL_CreateRGBSurfaceWithFormat(0, _wh.width, _wh.height, depth, static_cast<std::uint32_t>(fmt)); s != nullptr)
        return surface{*s};
    return {};
}

std::optional<surface> surface::create(void* const pixels, int const pitch, pixel_format_enum const fmt, int const depth, wh<int> const _wh) noexcept {
    if (auto const s = SDL_CreateRGBSurfaceWithFormatFrom(pixels, _wh.width, _wh.height, depth, pitch, static_cast<std::uint32_t>(fmt)); s != nullptr)
        return surface{*s};
    return {};
}

std::optional<surface> surface::create(null_term_string const file) noexcept {
    if (auto const s = IMG_Load(file.data()); s != nullptr)
        return surface{*s};
    return {};
}

constexpr surface::surface(surface&& other) noexcept 
    : surface_(std::exchange(other.surface_, nullptr))
{}

surface::~surface() noexcept {
    if (surface_)
        SDL_FreeSurface(surface_);
}

constexpr const_pixel_format_view surface::pixel_format() const noexcept { 
    SDL2_ASSERT(surface_->format != nullptr);
    return {*(surface_->format)}; 
}

constexpr int surface::width() const noexcept { return surface_->w; }
constexpr int surface::height() const noexcept { return surface_->h; }
constexpr int surface::pitch() const noexcept { return surface_->pitch; }
constexpr void const* surface::pixels() const noexcept { return surface_->pixels; }
constexpr void* surface::pixels() noexcept { return surface_->pixels; }
constexpr int surface::num_pixels() const noexcept { return height() * pitch(); }
constexpr void const* surface::userdata() const noexcept { return surface_->userdata; }
constexpr void* surface::userdata() noexcept { return surface_->userdata; }
constexpr void surface::set_userdata(void* const userdata) noexcept { surface_->userdata = userdata; }
constexpr rect<int> surface::clip_rect() const noexcept { return surface_->clip_rect; }
constexpr int surface::refcount() const noexcept { return surface_->refcount; }
constexpr int surface::refcount_add(int const amt) noexcept { 
    surface_->refcount += amt; 
    return surface_->refcount; 
}
constexpr int surface::refcount_sub(int const amt) noexcept { 
    surface_->refcount -= amt; 
    return surface_->refcount; 
}

int surface::refcount_atomic_load(std::memory_order const order) const noexcept {
    return std::atomic_ref{surface_->refcount}.load(order);
}
int surface::refcount_atomic_fetch_add(int const amt, std::memory_order const order) noexcept { 
    return std::atomic_ref{surface_->refcount}.fetch_add(amt, order);
}
int surface::refcount_atomic_fetch_sub(int const amt, std::memory_order const order) noexcept { 
    return std::atomic_ref{surface_->refcount}.fetch_sub(amt, order);
}

void surface::lock() noexcept { SDL_LockSurface(surface_); }
void surface::unlock() noexcept { SDL_UnlockSurface(surface_); }
bool surface::must_lock() const noexcept { return SDL_MUSTLOCK(surface_); }

// blit
bool surface::blit(rect<int> const& srcrect, surface& dst, rect<int>& dstrect) noexcept {
    return SDL_BlitSurface(surface_, srcrect.native_handle(), dst.native_handle(), dstrect.native_handle()) == 0;
}
bool surface::blit(surface& dst, rect<int>& dstrect) noexcept {
    return SDL_BlitSurface(surface_, nullptr, dst.native_handle(), dstrect.native_handle()) == 0;
}
bool surface::blit(surface& dst) noexcept {
    return SDL_BlitSurface(surface_, nullptr, dst.native_handle(), nullptr) == 0;
}
bool surface::blit(rect<int> const& srcrect, surface& dst) noexcept {
    return SDL_BlitSurface(surface_, srcrect.native_handle(), dst.native_handle(), nullptr) == 0;
}

// blit_scaled
bool surface::blit_scaled(rect<int> const& srcrect, surface& dst, rect<int>& dstrect) noexcept {
    return SDL_BlitScaled(surface_, srcrect.native_handle(), dst.native_handle(), dstrect.native_handle()) == 0;
}
bool surface::blit_scaled(surface& dst, rect<int>& dstrect) noexcept {
    return SDL_BlitScaled(surface_, nullptr, dst.native_handle(), dstrect.native_handle()) == 0;
}
bool surface::blit_scaled(surface& dst) noexcept {
    return SDL_BlitScaled(surface_, nullptr, dst.native_handle(), nullptr) == 0;
}
bool surface::blit_scaled(rect<int> const& srcrect, surface& dst) noexcept {
    return SDL_BlitScaled(surface_, srcrect.native_handle(), dst.native_handle(), nullptr) == 0;
}

// lower_blit
bool surface::lower_blit(rect<int> const& srcrect, surface& dst, rect<int>& dstrect) noexcept {
    return SDL_LowerBlit(surface_, const_cast<SDL_Rect*>(srcrect.native_handle()), dst.native_handle(), dstrect.native_handle()) == 0;
}
bool surface::lower_blit(surface& dst, rect<int>& dstrect) noexcept {
    return SDL_LowerBlit(surface_, nullptr, dst.native_handle(), dstrect.native_handle()) == 0;
}
bool surface::lower_blit(surface& dst) noexcept {
    return SDL_LowerBlit(surface_, nullptr, dst.native_handle(), nullptr) == 0;
}
bool surface::lower_blit(rect<int> const& srcrect, surface& dst) noexcept {
    return SDL_LowerBlit(surface_, const_cast<SDL_Rect*>(srcrect.native_handle()), dst.native_handle(), nullptr) == 0;
}

// lower_blit_scaled
bool surface::lower_blit_scaled(rect<int> const& srcrect, surface& dst, rect<int>& dstrect) noexcept {
    return SDL_LowerBlitScaled(surface_, const_cast<SDL_Rect*>(srcrect.native_handle()), dst.native_handle(), dstrect.native_handle()) == 0;
}
bool surface::lower_blit_scaled(surface& dst, rect<int>& dstrect) noexcept {
    return SDL_LowerBlitScaled(surface_, nullptr, dst.native_handle(), dstrect.native_handle()) == 0;
}
bool surface::lower_blit_scaled(surface& dst) noexcept {
    return SDL_LowerBlitScaled(surface_, nullptr, dst.native_handle(), nullptr) == 0;
}
bool surface::lower_blit_scaled(rect<int> const& srcrect, surface& dst) noexcept {
    return SDL_LowerBlitScaled(surface_, const_cast<SDL_Rect*>(srcrect.native_handle()), dst.native_handle(), nullptr) == 0;
}

bool surface::fill_rect(rect<int> const& rect, pixel_color const color) noexcept {
    return SDL_FillRect(surface_, rect.native_handle(), color) == 0;
}
bool surface::fill(pixel_color const color) noexcept {
    return SDL_FillRect(surface_, nullptr, color) == 0;
}
bool surface::fill_rects(std::span<rect<int> const> const rects, pixel_color const color) noexcept {
    return SDL_FillRects(surface_, rects.data()->native_handle(), static_cast<int>(rects.size()), color) == 0;
}

bool surface::convert(sdl2::pixel_format const& fmt) noexcept {
    return SDL_ConvertSurface(surface_, fmt.native_handle(), 0) != nullptr ? true : false;
}

std::optional<surface> surface::convert_to_new(sdl2::pixel_format const& fmt) const noexcept {
    if (auto const s = SDL_ConvertSurfaceFormat(surface_, static_cast<std::uint32_t>(fmt.format()), 0); s != nullptr)
        return surface{*s};
    return {};
}

std::optional<pixel_color> surface::color_key() const noexcept {
    std::uint32_t key{};
    if (SDL_GetColorKey(surface_, &key) == 0)
        return pixel_color{key};
    return {};
}

std::uint8_t surface::alpha_mod() const noexcept {
    std::uint8_t a{};
    [[maybe_unused]] auto const err = SDL_GetSurfaceAlphaMod(surface_, &a);
    SDL2_ASSERT(err == 0);
    return a;
}

sdl2::blend_mode surface::blend_mode() const noexcept {
    SDL_BlendMode mode{};
    [[maybe_unused]] auto const err = SDL_GetSurfaceBlendMode(surface_, &mode);
    SDL2_ASSERT(err == 0);
    return static_cast<sdl2::blend_mode>(mode);
}

rgb<std::uint8_t> surface::color_mod() const noexcept {
    rgb<std::uint8_t> ret;
    [[maybe_unused]] auto const err = SDL_GetSurfaceColorMod(surface_, &ret.r, &ret.g, &ret.b);
    SDL2_ASSERT(err == 0);
    return ret;
}

bool surface::set_clip_rect(rect<int> const& rect) noexcept {
    return SDL_SetClipRect(surface_, rect.native_handle()) == SDL_TRUE;
}

bool surface::disable_clipping() noexcept {
    return SDL_SetClipRect(surface_, nullptr) == SDL_TRUE;
}

bool surface::set_color_key(bool const enable, pixel_color const color) noexcept {
    return SDL_SetColorKey(surface_, static_cast<int>(enable), color);
}

bool surface::set_alpha_mod(std::uint8_t const alpha) noexcept {
    return SDL_SetSurfaceAlphaMod(surface_, alpha) == 0;
}

bool surface::set_blend_mode(sdl2::blend_mode const mode) noexcept {
    return SDL_SetSurfaceBlendMode(surface_, static_cast<SDL_BlendMode>(mode)) == 0;
}

bool surface::set_color_mode(rgb<std::uint8_t> const _rgb) noexcept {
    return SDL_SetSurfaceColorMod(surface_, _rgb.r, _rgb.g, _rgb.b) == 0;
}

bool surface::set_palette(palette_view const p) noexcept {
    return SDL_SetSurfacePalette(surface_, p.native_handle()) == 0;
}

bool surface::set_rle(bool const enable) noexcept {
    return SDL_SetSurfaceRLE(surface_, static_cast<int>(enable)) == 0;
}

bool surface::save_bmp_to_file(null_term_string const file) const noexcept {
    return SDL_SaveBMP(surface_, file.data()) == 0;
}

bool convert_pixels(wh<int> _wh, surface const& src, surface& dst) noexcept {
    return SDL_ConvertPixels(_wh.width, _wh.height,
                             static_cast<std::uint32_t>(src.pixel_format().format()), src.pixels(), src.pitch(),
                             static_cast<std::uint32_t>(dst.pixel_format().format()), dst.pixels(), dst.pitch()) == 0;
}