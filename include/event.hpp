#pragma once

#include <SDL2/SDL.h>

#include <chrono>
#include <concepts>
#include <optional>
#include <span>

namespace sdl2 {

template<class It, class Sent>
concept sdl_event_output_pair = std::contiguous_iterator<It> && std::output_iterator<It, SDL_Event> && std::sentinel_for<It, Sent>;

struct event_queue_t {
    /**
    * @brief
    * @param type
    * @return
    */
    static bool has(SDL_EventType const type) noexcept {
        return SDL_HasEvent(type) == SDL_TRUE;
    }

    /**
    * @brief
    * @param min
    * @param max
    * @return
    */
    static bool has(SDL_EventType const min, SDL_EventType const max) noexcept {
        return SDL_HasEvents(min, max) == SDL_TRUE;
    }

    /**
    * @brief
    */
    static void pump() noexcept {
        SDL_PumpEvents();
    }


    /**
     * @brief
     * @return 
     */
    static std::optional<SDL_Event> poll() noexcept {
        SDL_Event e{};
        if (SDL_PollEvent(&e))
            return e;
        return {};
    }

    enum class push_result : int {
        SUCCESS = 1, FILTERED = 0, FAILED = -1,
    };

    /**
    * @brief
    * @param e
    * @return
    */
    static push_result push(SDL_Event& e) noexcept {
        return static_cast<push_result>(SDL_PushEvent(&e));
    }

    /**
    * @brief
    * @param e
    * @return
    */
    static push_result push(SDL_Event&& e) noexcept {
        return static_cast<push_result>(SDL_PushEvent(&e));
    }

    /**
    * @brief
    * @param e
    * @return
    */
    static std::optional<std::size_t> add(std::span<SDL_Event> const events) noexcept {
        if (auto const num = SDL_PeepEvents(events.data(), events.size(), SDL_ADDEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT); num >= 0)
            return static_cast<std::size_t>(num);
        return {};
    } 

    /**
     * @brief
     * @param first
     * @param last
     * @param min
     * @param max 
     */
    template<class It, class Sent>
    requires sdl_event_output_pair<It, Sent>
    static std::optional<It> peek(It const first, Sent const last, SDL_EventType const min = SDL_FIRSTEVENT, SDL_EventType const max = SDL_LASTEVENT) noexcept {
        if (auto const num_events = SDL_PeepEvents(std::to_address(first), std::distance(first, last), SDL_PEEKEVENT, min, max); num_events >= 0)
            return std::make_optional<It>(first + num_events);
        return {};
    }

    /**
     * @brief
     * @param first
     * @param last
     * @param min
     * @param max 
     */
    template<class It, class Sent>
    requires sdl_event_output_pair<It, Sent>
    static std::optional<It> remove(It const first, Sent const last, SDL_EventType const min = SDL_FIRSTEVENT, SDL_EventType const max = SDL_LASTEVENT) noexcept {
        if (auto const num_events = SDL_PeepEvents(std::to_address(first), std::distance(first, last), SDL_GETEVENT, min, max); num_events >= 0)
            return std::make_optional<It>(first + num_events);
        return {};
    }

    /**
     * @brief
     * @return 
     */
    static std::optional<SDL_Event> wait() noexcept {
        SDL_Event e{};
        if (SDL_WaitEvent(&e))
            return e;
        return {};
    }

    /**
     * @brief
     * @param dur
     * @return 
     */
    template<class Rep, class Period>
    static std::optional<SDL_Event> wait_for(std::chrono::duration<Rep, Period> const& dur) noexcept {
        using sdl_dur_t = std::chrono::duration<int, std::milli>;
        SDL_Event e{};
        if (SDL_WaitEventTimeout(&e, std::chrono::round<sdl_dur_t>(dur).count()))
            return e;
        return {};
    }

    /**
     * @brief
     * @param tp
     * @return 
     */
    template<class Clock, class Dur>
    static std::optional<SDL_Event> wait_until(std::chrono::time_point<Clock, Dur> const& tp) noexcept {
        auto const now = std::chrono::time_point_cast<decltype(tp)>(Clock::now());
        if (tp < now)
            return {};
        return wait_for(tp - now);
    }  

    /**
     * @brief 
     */
    struct sentinel {};

    /**
     * @brief 
     */
    class iterator {
        SDL_Event e_{};
        bool done_ = false;

        friend class event_queue_t;

        iterator() noexcept
            : done_(SDL_PollEvent(&e_) == 0) {}

    public:
        using value_type = SDL_Event;
        using reference = SDL_Event&;

        /**
         * @brief
         * @return 
         */
        constexpr SDL_Event& operator*() noexcept {
            return e_;
        }

        /**
         * @brief
         * @return 
         */
        constexpr SDL_Event* operator->() noexcept {
            return &e_;
        }
        
        /**
         * @brief
         * @return 
         */
        iterator& operator++() noexcept {
            done_ = (SDL_PollEvent(&e_) == 0);
            return *this;
        }

        /**
         * @brief
         * @param sentinel
         * @return 
         */        
        constexpr bool operator!=(sentinel const) const noexcept {
            return !done_;
        }
    };

    /**
     * @brief
     * @return 
     */
    static iterator begin() noexcept { return {}; }

    /**
     * @brief
     * @return 
     */
    static sentinel end() noexcept { return {}; }
};

/**
 * @brief 
 */
inline constexpr event_queue_t event_queue{};

/*
SDL_AddEventWatch
SDL_DelEventWatch
SDL_EventState
SDL_FilterEvents
SDL_FlushEvent
SDL_FlushEvents
SDL_GetEventFilter
SDL_GetEventState
SDL_GetNumTouchDevices
SDL_GetNumTouchFingers
SDL_GetTouchDevice
SDL_GetTouchFinger
SDL_LoadDollarTemplates
SDL_QuitRequested
SDL_RecordGesture
SDL_RegisterEvents
SDL_SaveAllDollarTemplates
SDL_SaveDollarTemplate
SDL_SetEventFilter
SDL_WaitEvent
SDL_WaitEventTimeout
*/

} // namespace sdl2