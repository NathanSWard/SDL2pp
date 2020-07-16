#pragma once

#include <SDL2/SDL.h>
#include <concepts>
#include <cstdint>
#include <optional>
#include <span>
#include "enums.hpp"
#include "util.hpp"

namespace sdl2 {

namespace color {
    inline constexpr auto red = rgba<>{255, 0, 0, 255};
    inline constexpr auto orange = rgba<>{255, 165, 0, 255};
    inline constexpr auto yellow = rgba<>{255, 255, 0, 255};
    inline constexpr auto green = rgba<>{0, 128, 0, 255};
    inline constexpr auto blue = rgba<>{0, 0, 255, 255};
    inline constexpr auto purple = rgba<>{128, 0, 128, 255};
    inline constexpr auto black = rgba<>{0, 0, 0, 255};
    inline constexpr auto white = rgba<>{255, 255, 255, 255};
}

template<bool Const>
class basic_palette_view {
    SDL_Palette* palette_;

public:
    template<class P>
    requires std::same_as<SDL_Palette, std::remove_cvref_t<P>>
    constexpr basic_palette_view(P&& palette) noexcept : palette_(std::addressof(palette)) {}

    constexpr basic_palette_view(basic_palette_view const&) noexcept = default;
    constexpr basic_palette_view(basic_palette_view&&) noexcept = default;
    constexpr basic_palette_view& operator=(basic_palette_view const&) noexcept = default;
    constexpr basic_palette_view& operator=(basic_palette_view&&) noexcept = default;

    constexpr SDL_Palette* native_handle() const noexcept { return palette_; }

    template<bool B = Const>
    requires(B == false)
    constexpr std::span<SDL_Color> colors() noexcept { 
        return std::span{palette_->colors, static_cast<std::size_t>(palette_->ncolors)}; 
    }

    constexpr std::span<SDL_Color const> colors() const noexcept { 
        return std::span{palette_->colors, static_cast<std::size_t>(palette_->ncolors)}; 
    }
};

basic_palette_view(SDL_Palette&) -> basic_palette_view<false>;
basic_palette_view(SDL_Palette const&) -> basic_palette_view<true>;

using palette_view = basic_palette_view<false>;
using const_palette_view = basic_palette_view<true>;

class pixel_format {
    SDL_PixelFormat* fmt_;

public:
    static std::optional<pixel_format> create(pixel_format_enum const fmt) noexcept {
        if (auto const pf = SDL_AllocFormat(static_cast<std::uint32_t>(fmt)); pf != nullptr)
            return pixel_format{*pf};
        return {};
    }

    constexpr pixel_format(SDL_PixelFormat& fmt) noexcept : fmt_(std::addressof(fmt)) {}

    ~pixel_format() noexcept {
        if (fmt_)
            SDL_FreeFormat(fmt_);
    }

    constexpr pixel_format(pixel_format&& other) noexcept
        : fmt_(std::exchange(other.fmt_, nullptr))
    {}

    pixel_format(pixel_format const&) = delete;
    pixel_format& operator=(pixel_format const&) = delete;
    pixel_format& operator=(pixel_format&&) = delete;

    constexpr SDL_PixelFormat* native_handle() const noexcept { return fmt_; }

    constexpr pixel_format_enum format() const noexcept { return static_cast<pixel_format_enum>(fmt_->format); }

    constexpr bool has_palette() const noexcept { return fmt_->palette != nullptr; }

    constexpr palette_view palette() noexcept { 
        SDL2_ASSERT(has_palette());
        return palette_view{*(fmt_->palette)}; 
    }
    constexpr const_palette_view palette() const noexcept  { 
        SDL2_ASSERT(has_palette());
        return const_palette_view{*(fmt_->palette)}; 
    }
    constexpr std::uint8_t bits_per_pixel() const noexcept { return fmt_->BitsPerPixel; }
    constexpr std::uint8_t bytes_per_pixel() const noexcept { return fmt_->BytesPerPixel; }
    constexpr std::uint32_t rmask() const noexcept { return fmt_->Rmask; }
    constexpr std::uint32_t gmask() const noexcept { return fmt_->Gmask; }
    constexpr std::uint32_t bmask() const noexcept { return fmt_->Bmask; }
    constexpr std::uint32_t amask() const noexcept { return fmt_->Amask; }
};

template<bool Const>
class basic_pixel_format_view {
    SDL_PixelFormat* fmt_;

public:
    template<class PF>
    requires std::same_as<SDL_PixelFormat, std::remove_cvref_t<PF>>
    constexpr basic_pixel_format_view(PF&& fmt) noexcept : fmt_(std::addressof(fmt)) {}

    template<class PF>
    requires std::same_as<pixel_format, std::remove_cvref_t<PF>>
    constexpr basic_pixel_format_view(PF&& fmt) noexcept : fmt_(fmt.native_handle()) {}

    basic_pixel_format_view(std::nullptr_t) = delete;

    constexpr basic_pixel_format_view(basic_pixel_format_view const&) = default;
    constexpr basic_pixel_format_view(basic_pixel_format_view&&) = default;
    constexpr basic_pixel_format_view& operator=(basic_pixel_format_view const&) = default;
    constexpr basic_pixel_format_view& operator=(basic_pixel_format_view&&) = default;

    constexpr SDL_PixelFormat* native_handle() const noexcept { return fmt_; }

    constexpr pixel_format_enum format() const noexcept { return static_cast<pixel_format_enum>(fmt_->format); }

    constexpr bool has_palette() const noexcept {
        return fmt_->palette != nullptr;
    }

    template<bool B = Const>
    requires (B == false)
    constexpr palette_view palette() noexcept { 
        SDL2_ASSERT(has_palette());
        return palette_view{*(fmt_->palette)}; 
    }

    constexpr const_palette_view palette() const noexcept  { 
        SDL2_ASSERT(has_palette());
        return const_palette_view{*(fmt_->palette)};
    }

    constexpr std::uint8_t bits_per_pixel() const noexcept { return fmt_->BitsPerPixel; }
    constexpr std::uint8_t bytes_per_pixel() const noexcept { return fmt_->BytesPerPixel; }
    constexpr std::uint32_t rmask() const noexcept { return fmt_->Rmask; }
    constexpr std::uint32_t gmask() const noexcept { return fmt_->Gmask; }
    constexpr std::uint32_t bmask() const noexcept { return fmt_->Bmask; }
    constexpr std::uint32_t amask() const noexcept { return fmt_->Amask; }
};

basic_pixel_format_view(SDL_PixelFormat&) -> basic_pixel_format_view<false>;
basic_pixel_format_view(SDL_PixelFormat const&) -> basic_pixel_format_view<true>;
basic_pixel_format_view(pixel_format&) -> basic_pixel_format_view<false>;
basic_pixel_format_view(pixel_format const&) -> basic_pixel_format_view<true>;

using pixel_format_view = basic_pixel_format_view<false>;
using const_pixel_format_view = basic_pixel_format_view<true>;

class surface;

class pixel_color {
    std::uint32_t col_;
    friend class surface;

public:
    constexpr explicit pixel_color(std::uint32_t const col) noexcept : col_(col) {}

    constexpr explicit operator std::uint32_t() const noexcept { return col_; }
    constexpr std::uint32_t value() const noexcept { return col_; }

    explicit pixel_color(pixel_format const& fmt, rgb<std::uint8_t> const rgb) noexcept 
        : col_(SDL_MapRGB(fmt.native_handle(), rgb.r, rgb.g, rgb.b)) {}

    explicit pixel_color(pixel_format const& fmt, rgba<std::uint8_t> const rgba) noexcept 
        : col_(SDL_MapRGBA(fmt.native_handle(), rgba.r, rgba.g, rgba.b, rgba.a)) {}

    constexpr pixel_color(pixel_color const&) noexcept = default;
    constexpr pixel_color(pixel_color&&) noexcept = default;
    constexpr pixel_color& operator=(pixel_color const&) noexcept = default;
    constexpr pixel_color& operator=(pixel_color&&) noexcept = default;

    rgb<std::uint8_t> as_rgb(pixel_format const& fmt) const noexcept {
        rgb<std::uint8_t> _rgb{};
        SDL_GetRGB(col_, fmt.native_handle(), &_rgb.r, &_rgb.g, &_rgb.b);
        return _rgb;
    } 

   rgba<std::uint8_t> as_rgba(pixel_format const& fmt) const noexcept {
        rgba<std::uint8_t> _rgba{};
        SDL_GetRGBA(col_, fmt.native_handle(), &_rgba.r, &_rgba.g, &_rgba.b, &_rgba.a);
        return _rgba;
    } 
};

}