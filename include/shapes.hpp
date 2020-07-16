#pragma once

#include <SDL2/SDL.h>
#include <concepts>
#include <type_traits>

#include "utility.hpp"

namespace sdl2 {

template<class T>
concept sdl2_shape_rep = std::disjunction_v<std::is_same<T, float>, std::is_same<T, int>>;

template<sdl2_shape_rep T>
class rect {
    using sdl_rect_t = std::conditional_t<std::is_same_v<T, int>, SDL_Rect, SDL_FRect>;
    sdl_rect_t rect_;

public:
    constexpr rect(sdl_rect_t const& rect) noexcept
        : rect_{rect} {}

    constexpr rect(T const x = T(0), T const y = T(0), T const w = T(0), T const h = T(0)) noexcept
        : rect_{x, y, w, h} {}

    constexpr rect(rect const&) noexcept = default;
    constexpr rect(rect&&) noexcept = default;
    constexpr rect& operator=(rect const&) noexcept = default;
    constexpr rect& operator=(rect&&) noexcept = default;

    constexpr auto const& get() const noexcept { return rect_; }
    constexpr auto& get() noexcept { return rect_; }

    constexpr auto const& x() const noexcept { return rect_.x; }
    constexpr auto const& y() const noexcept { return rect_.y; }
    constexpr auto const& w() const noexcept { return rect_.w; }
    constexpr auto const& h() const noexcept { return rect_.h; }

    constexpr auto& x() noexcept { return rect_.x; }
    constexpr auto& y() noexcept { return rect_.y; }
    constexpr auto& w() noexcept { return rect_.w; }
    constexpr auto& h() noexcept { return rect_.h; }

    constexpr auto size() const noexcept { return rect_.w * rect_.h; }

    constexpr xy<T> top_left() const noexcept { return {rect_.x, rect_.y}; };
    constexpr xy<T> top_right() const noexcept { return {rect_.x + rect_.w, rect_.y}; };
    constexpr xy<T> bottom_left() const noexcept { return {rect_.x, rect_.y + rect_.h}; };
    constexpr xy<T> bottom_right() const noexcept { return {rect_.x + rect_.w, rect_.y + rect_.h}; };
    constexpr xy<T> center() const noexcept { 
        return {rect_.x + rect_.w / T{2}, rect_.y + rect_.h / T{2});
    }

    template<std::size_t I>
    constexpr auto& get() noexcept {
        if constexpr (I == 0) return x();
        else if constexpr (I == 1) return y();
        else if constexpr (I == 2) return w();
        else if constexpr (I == 3) return h();
        else static_assert(false, "invalid sdl2::rect::get<I>() index");
    }

    template<std::size_t I>
    constexpr auto const& get() const noexcept {
        if constexpr (I == 0) return x();
        else if constexpr (I == 1) return y();
        else if constexpr (I == 2) return w();
        else if constexpr (I == 3) return h();
        else static_assert(false, "invalid sdl2::rect::get<I>() index");
    }

};

template<sdl2_shape_rep T>
class point  {
    using sdl_point_t = std::conditional_t<std::is_same_v<T, int>, SDL_Point, SDL_FPoint>;
    sdl_point_t point_;

public:
    constexpr point(sdl_point_t const& p) noexcept
        : point_(p) {}

    constexpr point(T const x = T(0), T const y = T(0)) noexcept 
        : point_{x, y} {}

    constexpr point(point const&) noexcept = default;
    constexpr point(point&&) noexcept = default;
    constexpr point& operator=(point const&) noexcept = default;
    constexpr point& operator=(point&&) noexcept = default;

    constexpr auto const& get() const noexcept { return point_; }
    constexpr auto& get() noexcept { return point_; }

    constexpr auto const& x() const noexcept { return point_.x; }
    constexpr auto& x() noexcept { return point_.x; }
    constexpr auto const& y() const noexcept { return point_.y; }
    constexpr auto& y() noexcept { return point_.y; }

    template<std::size_t I>
    constexpr auto& get() noexcept {
        if constexpr (I == 0) return x();
        else if constexpr (I == 1) return y();
        else static_assert(false, "invalid sdl2::point::get<I>() index");
    }

    template<std::size_t I>
    constexpr auto const& get() const noexcept {
        if constexpr (I == 0) return x();
        else if constexpr (I == 1) return y();
        else static_assert(false, "invalid sdl2::point::get<I>() index");
    }
};

} // namespace sdl2

namespace std {
    template<class T>
    struct tuple_size<sdl2::rect<T>> {
        static constexpr std::size_t value = 4;
    };

    template<class T>
    struct tuple_size<sdl2::point<T>> {
        static constexpr std::size_t value = 2;
    };
} // namespace sdl2