#pragma once

#include <SDL2/SDL.h>

#include <chrono>
#include <concepts>
#include <optional>
#include <span>

#include "util.hpp"

namespace sdl2 {

template<class It, class Sent>
concept sdl_event_output_pair = std::contiguous_iterator<It> && std::output_iterator<It, SDL_Event> && std::sentinel_for<It, Sent>;

struct event_queue_t {
    /**
    * @brief Checks to see if an event type exists in the event queue.
    * @param type The event type to check for.
    * @return True if the event queue contains the type, false if not.
    */
    static bool has(SDL_EventType type) noexcept;

    /**
    * @brief Checks if an event exists in the event queue between a min and max.
    * @param min The minimum event type.
    * @param max The maximum event type.
    * @return True if an event exists between min/max, false if not.
    */
    static bool has(SDL_EventType min, SDL_EventType max) noexcept;

    /**
    * @brief Pumps the event loop, gathering events from input devices.
    * @note This should only be run in the thread that sets the video mode.
    */
    static void pump() noexcept;

    /**
     * @brief Poll for currently pending events.
     * @return An SDL_Event if one exists, an empty optional if no events exit in the queue.
     */
    static std::optional<SDL_Event> poll() noexcept;

    enum class push_result : int {
        SUCCESS = 1, FILTERED = 0, FAILED = -1,
    };

    /**
    * @brief Add an event to the event queue.
    * @param e The event to add.
    * @return A push_result specifying the result of attempting to push the event.
    */
    static push_result push(SDL_Event& e) noexcept;

    /**
    * @brief Add an event to the event queue.
    * @param e The event to add.
    * @return A push_result specifying the result of attempting to push the event.
    */
    static push_result push(SDL_Event&& e) noexcept;

    /**
    * @brief Add events to the event queue.
    * @param events The events too add to the event queue.
    * @return The number of event added to the queue, or an empty optional if an error occured.
    */
    static std::optional<std::size_t> add(std::span<SDL_Event> events) noexcept;

    /**
     * @brief Peek at events in the event queue.
     * @param first An iterator to the beginning of the output range to copy the events.
     * @param last An iterator to the end of the output range.
     * @param min The minimum event type to consider. 
     * @param max The maximum event type to consider.
     * @return An iterator to one-past the last event added, or `first` if no events exist/an error occured.
     */
    template<class It, class Sent>
    requires sdl_event_output_pair<It, Sent>
    static It peek(It first, Sent last, SDL_EventType min = SDL_FIRSTEVENT, SDL_EventType max = SDL_LASTEVENT) noexcept;

    /**
     * @brief Remove events from the event queue.
     * @param first An iterator to the beginning of the output range to move the events to.
     * @param last An iterator to the end of the output range.
     * @param min The minimum event type to consider. 
     * @param max The maximum event type to consider. 
     * @return An iterator to one-past the last event removed, or `first` if no events exist/an error occured.
     */
    template<class It, class Sent>
    requires sdl_event_output_pair<It, Sent>
    static It remove(It first, Sent last, SDL_EventType min = SDL_FIRSTEVENT, SDL_EventType max = SDL_LASTEVENT) noexcept;

    /**
     * @brief Wait indefinitely for the next available event.
     * @return An event, or an empty optional if an error occured.
     */
    static std::optional<SDL_Event> wait() noexcept;

    /**
     * @brief Wait a duration for the next available event.
     * @param dur The duration to wait for.
     * @return An event, or an empty optional if the function timedout/an error occured.
     */
    template<class Rep, class Period>
    static std::optional<SDL_Event> wait_for(std::chrono::duration<Rep, Period> const& dur) noexcept;

    /**
     * @brief Wait until a point in time for the next available event.
     * @param tp The time point to wait until.
     * @return An event, or an empty optional if the function timedout/an error occured. 
     */
    template<class Clock, class Dur>
    static std::optional<SDL_Event> wait_until(std::chrono::time_point<Clock, Dur> const& tp) noexcept;

    /**
     * @brief A placeholder sentinel struct for iterating the event queue.
     */
    struct sentinel {};

    /**
     * @brief An iterator over the SDL2 event queue.
     */
    class iterator {
        SDL_Event e_{};
        bool done_ = false;

        friend class event_queue_t;

        explicit iterator() noexcept
            : done_(SDL_PollEvent(&e_) == 0) {}

    public:
        using value_type = SDL_Event;
        using reference = SDL_Event&;
        using pointer = SDL_Event*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::input_iterator_tag;

        /**
         * @brief Access the current event.
         * @return The current SDL_Event.
         */
        constexpr SDL_Event& operator*() noexcept {
            return e_;
        }

        /**
         * @brief Get a pointer to the current event.
         * @return A pointer to the current SDL_Event.
         */
        constexpr SDL_Event* operator->() noexcept {
            return &e_;
        }
        
        /**
         * @brief Gets the next event from the event queue.
         * @return A reference to this iterator.
         */
        iterator& operator++() noexcept {
            done_ = (SDL_PollEvent(&e_) == 0);
            return *this;
        }

        /**
         * @brief Checks if the event queue it empty.
         * @param sentinel A placeholder value.
         * @return True if more events can be polled, false if no more events in the queue.
         */        
        constexpr bool operator!=(sentinel) const noexcept {
            return !done_;
        }
    };

    /**
     * @brief Get an iterator to the first element in the SDL event queue.
     * @return An iterator to the first element in the SDL event queue.
     */
    static iterator begin() noexcept { return iterator{}; }

    /**
     * @brief Gets a sentinel value for a != comparison to an iterator;
     * @return A sentinel object.
     */
    constexpr static sentinel end() noexcept { return {}; }

    /**
     * @brief Clears an event type from the event queue.
     * @param type The type of event to flush.
     */
    static void flush(SDL_EventType type) noexcept;

    /**
     * @brief Clears events between a range from the event queue.
     * @param min The minimum event type to be cleared.
     * @param max The maximum event type to be cleared.
     */
    static void flush(SDL_EventType min, SDL_EventType max) noexcept;

    enum class event_state : bool {
        ENABLED = SDL_ENABLE,
        DISABLED = SDL_DISABLE,
    };

    /**
     * @brief Get the event state of the event type.
     * @param type The event type to query.
     * @return The event type's state.
     */
    static event_state get_event_state(SDL_EventType type) noexcept;

    /**
     * @brief Get the event state of the event type.
     * @param type The event type to query.
     * @param state The event type's new state.
     * @return The event's previous state.
     */
    static event_state set_event_state(SDL_EventType type, event_state state) noexcept;

    /**
     * @brief Checks if an SDL_QUIT event is queued.
     * @return True if queued, false if not.
     */
    static bool quit_requested() noexcept;

    /**
     * @brief Add a callback to be triggered when an event is added to the event queue.
     * @param callback The callback.
     * @warning The lifetime of the callback must be maintined until `del_event_watch` is invoked on the callback.
     */
    template<class F>
    requires invocable_r<void, F, SDL_Event&>
    static void add_event_watch(F& callback) noexcept;

    /**
     * @brief Remove an event watch callback initally added with `add_event_watch`.
     * @param callback The callback to be deleted.
     */
    template<class F>
    requires invocable_r<void, F, SDL_Event&>
    static void del_event_watch(F& callback) noexcept;

    /**
     * @brief Set a filter to process all events before they change internal state and are posted to the event queue.
     * @param callback The filter function which returns true if the event should be added, false if the event should be ignored.
     * @warning The lifetime of the callback must be maintained until the event filter is changed.
     */
    template<class F>
    requires invocable_r<bool, F, SDL_Event&>
    static void set_event_filter(F& callback) noexcept;

    // TODO: Fix this functin all. Can't necessarily return a function_ref.
    ///**
    // * @brief Get the current event filter.
    // * @return The current event filter, or a null function_ref if no filter is set.
    // */
    //static function_ref<bool(SDL_Event&)> get_event_filter() noexcept;

    /**
     * @brief Runs the filter function on the current event queue.
     * @param fn A filter function returning false if the SDL_Event should be removed.
     */
    template<class F>
    requires invocable_r<bool, F, SDL_Event&>
    static void filter_events(F&& fn) noexcept;
};


/**
 * @brief The SDL2 "global" event queue. 
 * @note It's recommended to use this event queue rather than creating your own `event_queue_t` object.
 */
inline constexpr event_queue_t event_queue{};

namespace touch_events {

/**
 * @brief Get the number of registered touch devices.
 * @return The number of registered touch devices. 
 */
int get_num_touch_devices() noexcept;

/**
 * @brief Get the number of active fingers for a given touch device.
 * @param id The ID of a touch device.
 * @return The number of active fingers for the touch device on success or 0 on failure.
 */
int get_num_touch_fingers(SDL_TouchID id) noexcept;

/**
 * @brief Get the touch ID with the given index.
 * @param index The touch device index
 * @return The touch ID with the given index on success or 0 if the index is invalid. 
 */
SDL_TouchID get_touch_device(int index) noexcept;

/**
 * @brief Get the finger object for a specified touch device ID and finger index.
 * @param id The ID of the requested touch device.
 * @param index The index of the requested finger.
 * @return The SDL_Finger object or an empty optional_ref in no object at the given ID and index could be found.
 */
optional_ref<SDL_Finger> get_touch_finger(SDL_TouchID id, int index) noexcept;

/**
 * @brief Begins recording a gesture on the specified touch device.
 * @param id The touch device id.
 * @return True if succeeded, false if failed.
 */ 
bool record_gesture(SDL_TouchID id) noexcept;

/**
 * @brief Begins recording a gesture on all touch devices.
 * @return True if succeeded, false if failed.
 */ 
bool record_all_gestures() noexcept;

/**
 * @brief Load dollar gesture templates from a file.
 * @param id A touch id.
 * @param src An SDL_RWops to load from.
 * @return The number of loaded templates on success an number <= 0 on failure. 
 */
int load_dollar_templates(SDL_TouchID id, SDL_RWops& src) noexcept;

/**
 * @brief Saves all currently loaded dollar gesture templates.
 * @param dst An SDL_RWops to save to.
 * @return The number of saved templates or 0 on failure.
 */
int save_all_dollar_templates(SDL_RWops& dst) noexcept;

/**
 * @brief Save a currently loaded Dollar Gesture template.
 * @param id A gesture id.
 * @param dst An SDL_RWops to save to
 * @return True on success, false if failed.
 */
bool save_dollar_template(SDL_GestureID id, SDL_RWops& dst) noexcept;

} // namespace touch_events

// event_queue_t templated method implementations.
template<class It, class Sent>
requires sdl_event_output_pair<It, Sent>
It event_queue_t::peek(It const first, Sent const last, SDL_EventType const min, SDL_EventType const max) noexcept {
    auto const num_events = SDL_PeepEvents(std::to_address(first), std::distance(first, last), SDL_PEEKEVENT, min, max);
    return num_events > 0 ? first + num_events : first;
}

template<class It, class Sent>
requires sdl_event_output_pair<It, Sent>
It event_queue_t::remove(It const first, Sent const last, SDL_EventType const min, SDL_EventType const max) noexcept {
    auto const num_events = SDL_PeepEvents(std::to_address(first), std::distance(first, last), SDL_GETEVENT, min, max);
    return num_events > 0 ? first + num_events : first;
}

template<class Rep, class Period>
std::optional<SDL_Event> event_queue_t::wait_for(std::chrono::duration<Rep, Period> const& dur) noexcept {
    using sdl_dur_t = std::chrono::duration<int, std::milli>;
    SDL_Event e{};
    if (SDL_WaitEventTimeout(&e, std::chrono::round<sdl_dur_t>(dur).count()))
        return e;
    return {};
}

template<class Clock, class Dur>
std::optional<SDL_Event> event_queue_t::wait_until(std::chrono::time_point<Clock, Dur> const& tp) noexcept {
    auto const now = std::chrono::time_point_cast<decltype(tp)>(Clock::now());
    if (tp < now)
        return {};
    return wait_for(tp - now);
}  

namespace detail {

template<class F>
constexpr static int _event_watch_impl(void* fn, SDL_Event* event) noexcept(std::is_nothrow_invocable_r_v<void, F, SDL_Event&>) {
    (*static_cast<F*>(fn))(*event);
    return 0;
}

template<class F>
constexpr static int _event_filter_impl(void* fn, SDL_Event* event) noexcept(std::is_nothrow_invocable_r_v<bool, F, SDL_Event&>) {
    return static_cast<int>((*static_cast<F*>(fn))(*event));
}

} // namespace detail

template<class F>
requires invocable_r<void, F, SDL_Event&>
void event_queue_t::add_event_watch(F& fn) noexcept {
    SDL_AddEventWatch(detail::_event_watch_impl<F>, std::addressof(fn));
}

template<class F>
requires invocable_r<void, F, SDL_Event&>
void event_queue_t::del_event_watch(F& fn) noexcept {
    SDL_DelEventWatch(detail::_event_watch_impl<F>, std::addressof(fn));
}

template<class F>
requires invocable_r<bool, F, SDL_Event&>
void event_queue_t::set_event_filter(F& fn) noexcept {
    SDL_SetEventFilter(detail::_event_filter_impl<F>, std::addressof(fn));
}

template<class F>
requires invocable_r<bool, F, SDL_Event&>
void event_queue_t::filter_events(F&& fn) noexcept {
    SDL_FilterEvents(detail::_event_filter_impl<F>, std::addressof(fn));
}

} // namespace sdl2