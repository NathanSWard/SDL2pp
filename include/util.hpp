#pragma once

#include <memory>
#include <cassert>
#include <concepts>
#include <string>
#include <type_traits>

#define SDL2_ASSERT(b) assert(b)

namespace sdl2 {

template<class T = int>
struct xy {
    T x{};
    T y{};
};

template<class T = int>
struct wh {
    T width{};
    T height{};
};

template<class T = int>
struct tlbr {
    T top{};
    T left{};
    T bottom{};
    T right{};
};

template<class T = std::uint8_t>
struct rgb {
    T r{};
    T g{};
    T b{};
};

template<class T = std::uint8_t>
struct rgba {
    T r{};
    T g{};
    T b{};
    T a{};

    template<class U = T, std::enable_if_t<std::is_same_v<U, std::uint8_t>, int> = 0> 
    constexpr operator SDL_Color() const noexcept {
        return SDL_Color{r, g, b, a};
    }
};

 template<typename Fn> 
 class function_ref;
 
 template<typename R, typename ...Args>
 class function_ref<R(Args...)> {
    R (*callback)(std::intptr_t callable, Args... args) = nullptr;
    std::intptr_t callable;
 
    template<typename Callable>
    static R callback_fn(std::intptr_t callable, Args... args) {
        return (*reinterpret_cast<Callable*>(callable))(std::forward<Args>(args)...);
    }
 
 public:
    constexpr function_ref() noexcept = default;
 
    template<typename Callable, std::enable_if_t<std::is_same_v<std::remove_cvref_t<Callable>, function_ref>,int> = 0>
    constexpr function_ref(Callable&& callable)
        : callback(callback_fn<std::remove_reference_t<Callable>>)
        , callable(reinterpret_cast<std::intptr_t>(std::addressof(callable))) 
    {}
 
    constexpr R operator()(Args... args) const {
        return callback(callable, std::forward<Args>(args)...);
    }
 
    constexpr explicit operator bool() const noexcept { return callback; }
 };

struct nullopt_t { constexpr explicit nullopt_t() noexcept = default; };
inline constexpr nullopt_t nullopt{};

template<class T>
class optional_ref {
    T* ptr_ = nullptr;

public:
    constexpr optional_ref() noexcept = default;
    constexpr optional_ref(nullopt_t) noexcept {}

    template<class U>
    requires (!std::same_as<optional_ref, std::remove_cvref_t<U>>)
    constexpr optional_ref(U&& u) noexcept 
        : ptr_(std::addressof(u))
    {
        static_assert(std::is_lvalue_reference_v<U>, "");
    }

    optional_ref(optional_ref const&) noexcept = default;
    optional_ref(optional_ref&&) noexcept = default;

    optional_ref& operator=(optional_ref const&) = delete;
    optional_ref& operator=(optional_ref&&) = delete;

    constexpr explicit operator bool() const noexcept {
        return ptr_ != nullptr;
    }

    constexpr bool has_value() const noexcept {
        return static_cast<bool>(*this);
    }

    template<class U>
    constexpr T value_or(U&& fallback) const noexcept {
        return has_value() ? **this : static_cast<T>(std::forward<U>(fallback));
    } 

    constexpr T& operator*() noexcept {
        return *ptr_;
    }

    constexpr T const& operator*() const noexcept {
        return *ptr_;
    }

    constexpr T const* operator->() const noexcept {
        return ptr_;
    }

    constexpr T* operator->() noexcept {
        return ptr_;
    }
};

class null_term_string {
    char const* str_ = nullptr;
    std::size_t size_ = 0;

public:
    using value_type = char const;
    using reference = char const&;
    using iterator = char const*;
    using const_iterator = char const*;

    constexpr null_term_string() noexcept = default;

    constexpr null_term_string(null_term_string const&) noexcept = default;
    constexpr null_term_string(null_term_string&&) noexcept = default;
    constexpr null_term_string& operator=(null_term_string const&) noexcept = default;
    constexpr null_term_string& operator=(null_term_string&&) noexcept = default;

    null_term_string(std::string const& str) noexcept
        : str_(str.data()), size_(str.size()) {}
    
    constexpr null_term_string(char const* const str) noexcept 
        : str_(str), size_(std::char_traits<char>::length(str)) {}

    constexpr auto begin() const noexcept { return str_; }
    constexpr auto end() const noexcept { return str_ + size_; }
    constexpr std::size_t size() const noexcept { return size_; }
    constexpr auto data() const noexcept { return str_; }
};

} // namespace sdl2