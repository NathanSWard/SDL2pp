#include "../include/window.hpp"

using namespace sdl2;

std::optional<window> window::create(null_term_string const title, xy<int> const xy,
                                    wh<int> const wh, window_flags const flgs) noexcept 
{
    if (auto const win = SDL_CreateWindow(title.data(), xy.x, xy.y, wh.width, wh.height, static_cast<std::uint32_t>(flgs)); win != nullptr)
        return window{*win};
    return {};
}

std::optional<window> window::copy(window const& other) noexcept {
    auto const [x, y] = other.position();
    auto const [w, h] = other.size();
    if (auto const win = SDL_CreateWindow(other.title().data(), x, y, w, h, static_cast<std::uint32_t>(other.flags())); win != nullptr)
        return window{*win};
    return {};
}

window::~window() noexcept { 
    if (window_) 
        SDL_DestroyWindow(window_); 
}

void window::destroy() noexcept {
    SDL_DestroyWindow(window_);
    window_ = nullptr;
}

std::optional<tlbr<int>> window::borders_size() noexcept {
    int top = 0, left = 0, bottom = 0, right = 0;
    if (SDL_GetWindowBordersSize(window_, &top, &left, &bottom, &right) == 0)
        return tlbr<int>{top, left, bottom, right};
    return {};
}

float window::brightness() const noexcept { return SDL_GetWindowBrightness(window_); }

//SDL_GetWindowData

int window::display_index() const noexcept { return SDL_GetWindowDisplayIndex(window_); }

std::optional<SDL_DisplayMode> window::display_mode() const noexcept { 
    if (SDL_DisplayMode dm{}; SDL_GetWindowDisplayMode(window_, &dm) == 0)
        return dm;
    return {};
}

window_flags window::flags() const noexcept {
    return static_cast<window_flags>(SDL_GetWindowFlags(window_));
}

// SDL_GetWindowFromID

rgb<std::array<std::uint16_t, 256>> window::gamma_ramp() const noexcept {
    rgb<std::array<std::uint16_t, 256>> info{};
    [[maybe_unused]] auto const err = SDL_GetWindowGammaRamp(window_, info.r.data(), info.g.data(), info.b.data());
    SDL2_ASSERT(err >= 0);
    return info;
}

bool window::is_grabbed() const noexcept {
    return SDL_GetWindowGrab(window_) == SDL_TRUE;
}

std::uint32_t window::id() const noexcept {
    return SDL_GetWindowID(window_);
}

wh<int> window::maximum_size() const noexcept {
    int w = 0, h = 0;
    SDL_GetWindowMaximumSize(window_, &w, &h);
    return wh<int>{w, h};
}

wh<int> window::minimum_size() const noexcept {
    int w = 0, h = 0;
    SDL_GetWindowMinimumSize(window_, &w, &h);
    return wh<int>{w, h};
}

float window::opacity() const noexcept {
    float o{};
    [[maybe_unused]] auto const err = SDL_GetWindowOpacity(window_, &o);
    SDL2_ASSERT(err == 0);
    return o;
}

pixel_format_enum window::pixel_format() const noexcept {
    return static_cast<pixel_format_enum>(SDL_GetWindowPixelFormat(window_));
}

xy<int> window::position() const noexcept {
    int x{}, y{};
    SDL_GetWindowPosition(window_, &x, &y);
    return {x, y};
}

wh<int> window::size() const noexcept {
    int w{}, h{};
    SDL_GetWindowSize(window_, &w, &h);
    return{w, h};
}

// SDL_GetWindowSurface - TODO

std::string_view window::title() const noexcept {
    return std::string_view{SDL_GetWindowTitle(window_)};
}

std::optional<SDL_SysWMinfo> window::wm_info() const noexcept {
    if (SDL_SysWMinfo info{}; SDL_GetWindowWMInfo(window_, &info) == SDL_TRUE)
        return {info};
    return {};
}

void window::hide() noexcept {
    SDL_HideWindow(window_);
}

void window::maximize() noexcept {
    SDL_MaximizeWindow(window_);
}

void window::minimize() noexcept {
    SDL_MinimizeWindow(window_);
}

void window::raise() noexcept {
    SDL_RaiseWindow(window_);
}

void window::restore() noexcept {
    SDL_RestoreWindow(window_);
}

void window::set_bordered(bool const bordered) noexcept {
    SDL_SetWindowBordered(window_, static_cast<SDL_bool>(bordered));
}

bool window::set_brightness(float const brightness) noexcept {
    return SDL_SetWindowBrightness(window_, brightness) == 0;
}

// SDL_SetWindowData

bool window::set_display_mode(SDL_DisplayMode const& dm) noexcept {
    return SDL_SetWindowDisplayMode(window_, &dm) == 0;
}

bool window::set_display_mode() noexcept {
    return SDL_SetWindowDisplayMode(window_, nullptr) == 0;
}

bool window::set_fullscreen(fullscreen_flags const flags) noexcept {
    return SDL_SetWindowFullscreen(window_, static_cast<std::uint32_t>(flags)) == 0;
}

bool window::set_gamma_ramp(std::span<std::uint16_t const, 256> const r, 
                    std::span<std::uint16_t const, 256> const g, 
                    std::span<std::uint16_t const, 256> const b) noexcept {
    return SDL_SetWindowGammaRamp(window_, r.data(), g.data(), b.data()) == 0;
}

void window::set_grabbed(bool const grabbed) noexcept {
    SDL_SetWindowGrab(window_, static_cast<SDL_bool>(grabbed));
}

template<bool Const>
static SDL_HitTestResult _hit_test_wrapper(SDL_Window* win, SDL_Point const* const area, void* const func) {
    SDL2_ASSERT(win != nullptr && area != nullptr && func != nullptr);
    if constexpr (Const) {
        auto const fn = reinterpret_cast<function_ref<SDL_HitTestResult(window const&, point<int>)> const*>(func);
        return (*fn)(*reinterpret_cast<window const*>(win), point<int>{*area});
    }
    else {
        auto const fn = reinterpret_cast<function_ref<SDL_HitTestResult(window&, point<int>)> const*>(func);
        return (*fn)(*reinterpret_cast<window*>(win), point<int>{*area});
    }
}

bool window::set_hit_test(function_ref<SDL_HitTestResult(window&, point<int>)> const fn) noexcept {
    return SDL_SetWindowHitTest(window_, _hit_test_wrapper<false>, const_cast<void*>(static_cast<void const*>(std::addressof(fn)))) == 0;
}

bool window::set_hit_test(function_ref<SDL_HitTestResult(window const&, point<int>)> const fn) noexcept {
    return SDL_SetWindowHitTest(window_, _hit_test_wrapper<true>, const_cast<void*>(static_cast<void const*>(std::addressof(fn)))) == 0;
}

void window::set_icon(surface const& s) noexcept {
    SDL_SetWindowIcon(window_, s.native_handle());
}

bool window::set_input_focus() noexcept {
    return SDL_SetWindowInputFocus(window_) == 0;
}

void window::set_maximum_size(wh<int> const wh) noexcept {
    SDL2_ASSERT(wh.width > 0 && wh.height > 0);
    SDL_SetWindowMaximumSize(window_, wh.width, wh.height);
}

void window::set_minimum_size(wh<int> const wh) noexcept {
    SDL2_ASSERT(wh.width > 0 && wh.height > 0);
    SDL_SetWindowMinimumSize(window_, wh.width, wh.height);
}

bool window::set_modal_for(window const& parent) noexcept {
    return SDL_SetWindowModalFor(window_, parent.window_);
}

bool window::set_opacity(float const opacity) noexcept {
    return SDL_SetWindowOpacity(window_, opacity) == 0;
}

void window::set_position(xy<int> const xy) noexcept {
    SDL_SetWindowPosition(window_, xy.x, xy.y);
}

void window::set_resizable(bool const resizable) noexcept {
    SDL_SetWindowResizable(window_, static_cast<SDL_bool>(resizable));
}

void window::set_size(wh<int> const wh) noexcept {
    SDL2_ASSERT(wh.width > 0 && wh.height > 0);
    SDL_SetWindowSize(window_, wh.width, wh.height);
}

void window::set_title(null_term_string const title) noexcept {
    SDL_SetWindowTitle(window_, title.data());
}

bool window::show_simple_message_box(message_box_flags const flags, null_term_string const title, null_term_string const message) noexcept {
    return SDL_ShowSimpleMessageBox(static_cast<std::uint32_t>(flags), title.data(), message.data(), window_) == 0;
}

void window::show() noexcept {
    SDL_ShowWindow(window_);
}

bool window::update_surface() noexcept {
    return SDL_UpdateWindowSurface(window_) == 0;
}

bool window::update_surface_rects(std::span<rect<int> const> const rects) noexcept {
    return SDL_UpdateWindowSurfaceRects(window_, rects.data()->native_handle(), rects.size());
}