#pragma once

#include "SDL.h"

#include <cassert>
#include <concepts>
#include <memory>
#include <string>
#include <type_traits>

#define SDL2_ASSERT(b) assert(b)

namespace sdl2 {

/**
 * @brief X and Y positional helper struct.
 * @tparam T representation of values.
 */
template<class T = int>
struct xy {
    T x{};
    T y{};
};

/**
 * @brief Width and Height helper struct.
 * @tparam T representation of values.
 */
template<class T = int>
struct wh {
    T width{};
    T height{};
};

/**
 * @brief Top, Left, Bottom, Right helper struct.
 * @tparam T representation of values.
 */
template<class T = int>
struct tlbr {
    T top{};
    T left{};
    T bottom{};
    T right{};
};

/**
 * @brief RGB helper struct.
 * @tparam T representation of rgb values.
 */
template<class T = std::uint8_t>
struct rgb {
    T r{};
    T g{};
    T b{};
};

/**
 * @brief RGBA helper struct.
 * @tparam T representation of rgba values.
 */
template<class T = std::uint8_t>
struct rgba {
    T r{};
    T g{};
    T b{};
    T a{};

    /**
     * @brief Convert rgba into a SDL_Color.
     * @return SDL_Color struct.
     * @note Only enabled if T is std::uint8_t.
     */
    template<class U = T> 
    requires std::same_as<U, std::uint8_t>
    constexpr operator SDL_Color() const noexcept {
        return SDL_Color{r, g, b, a};
    }
};


template<typename Fn> 
class function_ref;

template<class R, class Fn, class... Args>
concept invocable_r = std::is_invocable_r_v<R, Fn, Args...>;

/**
 * @brief A non-owning callble wrapper.
 * @tparam R The return type.
 * @tparam Args The argument types.
 */ 
template<typename R, typename ...Args>
class function_ref<R(Args...)> {
    R (*callback)(std::intptr_t callable, Args... args) = nullptr;
    std::intptr_t callable{};

    template<typename Callable>
    static R callback_fn(std::intptr_t callable, Args... args) {
        return (*reinterpret_cast<Callable*>(callable))(std::forward<Args>(args)...);
    }

public:
    /**
     * @brief Default constructor.
     */
    constexpr function_ref() noexcept = default;

    /**
     * @brief Constructor taking a callable object.
     * @param callable A callable object to store.
     */
    template<typename Callable>
    requires (!std::same_as<function_ref, std::remove_cvref_t<Callable>> && invocable_r<R, Callable, Args...>)
    constexpr function_ref(Callable&& callable) noexcept
        : callback(callback_fn<std::remove_reference_t<Callable>>)
        , callable(reinterpret_cast<std::intptr_t>(std::addressof(callable))) 
    {}

    /**
     * @brief Invoke the function object.
     * @param args The arguments to forward to the held callable.
     * @return The result of the held callable.
     */
    constexpr R operator()(Args... args) const {
        return callback(callable, std::forward<Args>(args)...);
    }

    /**
     * @brief Bool conversion operator.
     * @return A bool indicating if this referes to a valid function object.
     */
    constexpr explicit operator bool() const noexcept { return callback; }
};

/**
 * @brief Helper class for optional_ref.
 * This class is used to construct an optional that does not contain its `T` type.
 */ 
struct nullopt_t { constexpr explicit nullopt_t() noexcept = default; };
inline constexpr nullopt_t nullopt{};

/**
 * @brief An non-rebindable optional reference container.
 */ 
template<class T>
class optional_ref {
    T* ptr_ = nullptr;

public:
    /**
     * @brief Default constructor.
     * Constructs an empty optional_ref.
     */
    constexpr optional_ref() noexcept = default;

    /**
     * @brief Construct an empty optional_ref.
     * @param _ A nullopt_t.
     */
    constexpr optional_ref(nullopt_t) noexcept {}

    /**
     * @brief Construct a non-empty optional_ref.
     * @param u The value which this optional_ref refers to.
     * @note The passed in value must be an lvalue reference. 
     */
    template<class U>
    requires (!std::same_as<optional_ref, std::remove_cvref_t<U>>)
    constexpr optional_ref(U&& u) noexcept 
        : ptr_(std::addressof(u))
    {
        static_assert(std::is_lvalue_reference_v<U>, "");
    }

    /**
     * @brief Copy constructor defaulted.
     */
    optional_ref(optional_ref const&) noexcept = default;

    /**
     * @brief Move constructor defaulted.
     */
    optional_ref(optional_ref&&) noexcept = default;

    /**
     * @brief Copy assignment deleted.
     */
    optional_ref& operator=(optional_ref const&) = delete;

    /**
     * @brief Move assignment deleted.
     */
    optional_ref& operator=(optional_ref&&) = delete;

    /**
     * @brief Bool conversion operator.
     * @return A bool denoting if this optional_ref holds a value.
     */
    constexpr explicit operator bool() const noexcept {
        return ptr_ != nullptr;
    }

    /**
     * @brief Check if this has a value.
     * @return A bool denoting if this optional_ref holds a value.
     */
    constexpr bool has_value() const noexcept {
        return static_cast<bool>(*this);
    }

    /**
     * @brief Return a copy of the refered to value or a fallback is no value is held.
     * @param fallback The value to use if *this does not hold a value.
     * @return Either the held value or the fallback.
     */
    template<class U>
    constexpr T value_or(U&& fallback) const noexcept {
        return has_value() ? **this : static_cast<T>(std::forward<U>(fallback));
    } 

    /**
     * @brief Access the held value.
     * @return A reference to the held value.
     * @note If *this does not hold a value, calling this is UB.
     */
    constexpr T& operator*() noexcept {
        return *ptr_;
    }

    /**
     * @brief Access the held value.
     * @return A const reference to the held value.
     * @note If *this does not hold a value, calling this is UB.
     */
    constexpr T const& operator*() const noexcept {
        return *ptr_;
    }

    /**
     * @brief Access the held value.
     * @return A const pointer to the held value or nullptr is no value is held.
     */
    constexpr T const* operator->() const noexcept {
        return ptr_;
    }

    /**
     * @brief Access the held value.
     * @return A pointer to the held value or nullptr is no value is held.
     */
    constexpr T* operator->() noexcept {
        return ptr_;
    }
};

/**
 * @brief A wrapper of a null-terminated string.
 */ 
class null_term_string {
    char const* str_ = "";
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

    /**
     * @brief Construct from a std::string.
     * @param str A std::string object.
     */
    null_term_string(std::string const& str) noexcept
        : str_(str.data()), size_(str.size()) {}
    
    /**
     * @brief Construct from a const char*.
     * @param str A null-terminate c-string.
     */
    constexpr null_term_string(char const* const str) noexcept 
        : str_(str), size_(std::char_traits<char>::length(str)) {}

    /**
     * @brief begin() iterator interface.
     * @return Pointer to the first character.
     */
    constexpr auto begin() const noexcept { return str_; }

    /**
     * @brief end() iterator interface.
     * @return Pointer to the one past-the-last character.
     */
    constexpr auto end() const noexcept { return str_ + size_; }

    /**
     * @brief Get the size of the string.
     * @return The size of the string.
     */
    constexpr std::size_t size() const noexcept { return size_; }

    /**
     * @brief Get access to the underlying const char*.
     * @return Pointer to the first character.
     */
    constexpr auto data() const noexcept { return str_; }
};

template<class...>
struct always_false {
    static constexpr bool value = std::false_type::value;
};

} // namespace sdl2