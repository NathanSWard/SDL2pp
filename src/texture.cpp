#include "texture.hpp"
#include "renderer.hpp"

#include <SDL2/SDL_image.h>

using namespace sdl2;

std::optional<texture> texture::create(renderer& r, pixel_format_enum const format, texture_access const access, wh<int> const wh) noexcept {
    if (auto const t = SDL_CreateTexture(r.native_handle(), static_cast<std::uint32_t>(format), static_cast<int>(access), wh.width, wh.height); t != nullptr)
        return texture{t};
    return {};
}

std::optional<texture> texture::create(renderer& r, surface const& s) noexcept {
    if (auto const t = SDL_CreateTextureFromSurface(r.native_handle(), s.native_handle()); t != nullptr)
        return texture{t};
    return {};
}

std::optional<texture> texture::create(renderer& r, null_term_string file) noexcept {
    if (auto const s = surface::create(file); s)
        if (auto const t = SDL_CreateTextureFromSurface(r.native_handle(), s->native_handle()); t != nullptr)
            return texture{t};
    return {};
}

constexpr texture::texture(texture&& other) noexcept 
    : texture_(std::exchange(other.texture_, nullptr)) {}

texture::~texture() noexcept { 
    if (texture_) 
        SDL_DestroyTexture(texture_); 
}

void texture::destroy() noexcept {
    SDL_DestroyTexture(texture_);
    texture_ = nullptr;
}

texture_lock texture::lock() noexcept {
    SDL2_ASSERT(access() == texture_access::STREAMING);
    void* pixels{};
    int pitch{};
    SDL_LockTexture(texture_, nullptr, &pixels, &pitch);
    return {texture_, pixels, pitch};
}

texture_lock texture::lock(rect<int> const& rect) noexcept {
    SDL2_ASSERT(access() == texture_access::STREAMING);
    void* pixels{};
    int pitch{};
    SDL_LockTexture(texture_, &rect.get(), &pixels, &pitch);
    return {texture_, pixels, pitch};
}

std::uint8_t texture::alpha_mod() const noexcept {
    std::uint8_t alpha{};
    [[maybe_unused]] auto const err = SDL_GetTextureAlphaMod(texture_, &alpha);
    SDL2_ASSERT(err == 0);
    return alpha;
}

sdl2::blend_mode texture::blend_mode() const noexcept {
    SDL_BlendMode bm = static_cast<SDL_BlendMode>(sdl2::blend_mode::INVALID);
    [[maybe_unused]] auto const err = SDL_GetTextureBlendMode(texture_, &bm);
    SDL2_ASSERT(err == 0);
    return static_cast<sdl2::blend_mode>(bm);
}

rgb<std::uint8_t> texture::color_mode() const noexcept {
    rgb<std::uint8_t> values;
    [[maybe_unused]] auto const err = SDL_GetTextureColorMod(texture_, &values.r, &values.g, &values.b);
    SDL2_ASSERT(err == 0);
    return values;
}

texture::texture_query texture::query() const noexcept {
    std::uint32_t format{};
    int access{}, width{}, height{};
    texture_query ret;
    [[maybe_unused]] auto const err = SDL_QueryTexture(texture_, &format, &access, &width, &height);
    SDL2_ASSERT(err == 0);
    return texture_query{static_cast<pixel_format_enum>(format), static_cast<texture_access>(access), width, height};
}

wh<int> texture::size() const noexcept {
    wh<int> ret;
    [[maybe_unused]] auto const err = SDL_QueryTexture(texture_, nullptr, nullptr, &ret.width, &ret.height);
    SDL2_ASSERT(err == 0);
    return ret;
}

pixel_format_enum texture::format() const noexcept {
    std::uint32_t format{};
    [[maybe_unused]] auto const err = SDL_QueryTexture(texture_, &format, nullptr, nullptr, nullptr);
    SDL2_ASSERT(err == 0);
    return static_cast<pixel_format_enum>(format);
}

texture_access texture::access() const noexcept {
    int access{};
    [[maybe_unused]] auto const err = SDL_QueryTexture(texture_, nullptr, &access, nullptr, nullptr);
    SDL2_ASSERT(err == 0);
    return static_cast<texture_access>(access);
}

bool texture::set_alpha_mode(std::uint8_t const alpha) noexcept {
    return SDL_SetTextureAlphaMod(texture_, alpha) == 0;
}

bool texture::set_blend_mode(sdl2::blend_mode const mode) noexcept {
    return SDL_SetTextureBlendMode(texture_, static_cast<SDL_BlendMode>(mode)) == 0;
}

bool texture::set_color_mod(rgb<> const mod) noexcept {
    return SDL_SetTextureColorMod(texture_, mod.r, mod.g, mod.b);
}

bool texture::update(rect<int> const& rect, std::span<pixel_color const> const pixels, int const pitch) noexcept {
    return SDL_UpdateTexture(texture_, &rect.get(), pixels.data(), pitch) == 0;
}

bool texture::update(std::span<pixel_color const> const pixels, int const pitch) noexcept {
    return SDL_UpdateTexture(texture_, nullptr, pixels.data(), pitch) == 0;
}

bool texture::update_yuv(rect<int> const& rect, 
                         std::uint8_t const* yplane, int const ypitch,
                         std::uint8_t const* uplane, int const upitch,
                         std::uint8_t const* vplane, int const vpitch) noexcept
{
    return SDL_UpdateYUVTexture(texture_, &rect.get(), yplane, ypitch, uplane, upitch, vplane, vpitch) == 0;
}

bool texture::update_yuv(std::uint8_t const* yplane, int const ypitch,
                         std::uint8_t const* uplane, int const upitch,
                         std::uint8_t const* vplane, int const vpitch) noexcept
{
    return SDL_UpdateYUVTexture(texture_, nullptr, yplane, ypitch, uplane, upitch, vplane, vpitch) == 0;
}