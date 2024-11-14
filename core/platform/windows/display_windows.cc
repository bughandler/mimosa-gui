#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <core/base/log.h>
#include <core/base/string_helper.h>
#include <shellscalingapi.h>
#include "display_windows.h"

#define UXTHEME_DLL_NAME L"uxtheme.dll"
#define SHCORE_DLL_NAME  L"Shcore.dll"

using ShouldAppsUseDarkModeFn = BOOL(WINAPI *)();
using GetImmersiveColorFromColorSetExFn = DWORD(WINAPI *)(UINT, UINT, bool, UINT);
using GetImmersiveColorTypeFromNameFn = int(WINAPI *)(const wchar_t *);
using GetImmersiveUserColorSetPreferenceFn = int(WINAPI *)(BOOL, BOOL);
using GetDpiForMonitorFn = HRESULT(WINAPI *)(HMONITOR, MONITOR_DPI_TYPE, UINT *, UINT *);

namespace mimosa {

HMODULE                                     gWinModule = nullptr;
static ShouldAppsUseDarkModeFn              ShouldAppsUseDarkMode = nullptr;
static GetImmersiveColorFromColorSetExFn    GetImmersiveColorFromColorSetEx = nullptr;
static GetImmersiveColorTypeFromNameFn      GetImmersiveColorTypeFromName = nullptr;
static GetImmersiveUserColorSetPreferenceFn GetImmersiveUserColorSetPreference = nullptr;
static GetDpiForMonitorFn                   GetDpiForMonitor_ = nullptr;
const float                                 kDefaultDPI_F = 96.f;
const float                                 kDefaultDPI = 96;

static LRESULT CALLBACK _WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    DisplayServerWindows *w = static_cast<DisplayServerWindows *>(PlatformDisplayServer::get_singleton());
    if (w) {
        return w->WindowProc(hwnd, uMsg, wParam, lParam);
    } else {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

LRESULT DisplayServerWindows::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool DisplayServerWindows::_register_window_class() {
    WNDCLASSEXW  regData = {0};
    std::wstring wClsName = utf8_to_utf16(windowClassName_);

    // initialize WNDCLASSEXW
    regData.cbSize = sizeof(regData);
    regData.lpfnWndProc = _WindowProc;
    regData.lpszClassName = wClsName.c_str();
    regData.hInstance = gWinModule ? gWinModule : GetModuleHandleW(nullptr);

    if (!RegisterClassExW(&regData)) {
        LOG_ERROR("Unable to register window class, error=%d\n", GetLastError());
        return false;
    }
    return true;
}

bool DisplayServerWindows::initialize() {
    HMODULE uxThemeModule = GetModuleHandleW(UXTHEME_DLL_NAME);
    if (!uxThemeModule) {
        uxThemeModule = LoadLibraryW(UXTHEME_DLL_NAME);
    }

    if (uxThemeModule) {
        ShouldAppsUseDarkMode = (ShouldAppsUseDarkModeFn)GetProcAddress(uxThemeModule, "ShouldAppsUseDarkMode");
        GetImmersiveColorFromColorSetEx =
            (GetImmersiveColorFromColorSetExFn)GetProcAddress(uxThemeModule, "GetImmersiveColorFromColorSetEx");
        GetImmersiveColorTypeFromName =
            (GetImmersiveColorTypeFromNameFn)GetProcAddress(uxThemeModule, "GetImmersiveColorTypeFromName");
        GetImmersiveUserColorSetPreference =
            (GetImmersiveUserColorSetPreferenceFn)GetProcAddress(uxThemeModule, "GetImmersiveUserColorSetPreference");
    }

    HMODULE shcoreModule = GetModuleHandleW(SHCORE_DLL_NAME);
    if (!shcoreModule) {
        shcoreModule = LoadLibraryW(SHCORE_DLL_NAME);
    }
    if (shcoreModule) {
        GetDpiForMonitor_ = (GetDpiForMonitorFn)GetProcAddress(shcoreModule, "GetDpiForMonitor");
    }

    return true;
}

bool DisplayServerWindows::is_feature_supported(Feature feature) const {
    switch (feature) {
    case Feature::kVirtualKeyboard:
    case Feature::kGlobalMenu: {
        return false;
    } break;
    }
    return true;
}

bool DisplayServerWindows::is_dark_mode() const {
    return ShouldAppsUseDarkMode && ShouldAppsUseDarkMode();
}

Color DisplayServerWindows::get_system_accent_color() const {
    if (!GetImmersiveColorFromColorSetEx || !GetImmersiveUserColorSetPreference || !GetImmersiveColorTypeFromName) {
        return {};
    }

    int argb = GetImmersiveColorFromColorSetEx((UINT)GetImmersiveUserColorSetPreference(false, false),
                                               GetImmersiveColorTypeFromName(L"ImmersiveSystemAccent"),
                                               false,
                                               0);
    return {argb & 0xFF, (argb & 0xFF00) >> 8, (argb & 0xFF0000) >> 16, (int)((argb & 0xFF000000) >> 24)};
}

Color DisplayServerWindows::get_system_base_color() const {
    if (!GetImmersiveColorFromColorSetEx || !GetImmersiveUserColorSetPreference || !GetImmersiveColorTypeFromName) {
        return {};
    }

    const wchar_t *colorName = is_dark_mode() ? L"ImmersiveDarkChromeMediumLow" : L"ImmersiveLightChromeMediumLow";
    int            argb = GetImmersiveColorFromColorSetEx((UINT)GetImmersiveUserColorSetPreference(false, false),
                                               GetImmersiveColorTypeFromName(colorName),
                                               false,
                                               0);
    return {argb & 0xFF, (argb & 0xFF00) >> 8, (argb & 0xFF0000) >> 16, (int)((argb & 0xFF000000) >> 24)};
}

struct EnumMonitorContext {
    enum class Action {
        kCount,
        kCheckPrimary,
        kMatchMonitorHandle,
        kGetMonitorInfoByOffset,
        kGetMonitorInfoByHandle,
    };
    Action   action = Action::kCount;
    int      offset = 0;
    HMONITOR handle = nullptr;
    HDC      dc = nullptr;
    RECT     rect = {0};
    bool     matched = false;
};

BOOL CALLBACK _MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
    EnumMonitorContext *ctx = (EnumMonitorContext *)dwData;
    switch (ctx->action) {
    case EnumMonitorContext::Action::kCount: {
        ++ctx->offset;
    } break;
    case EnumMonitorContext::Action::kCheckPrimary: {
        if (lprcMonitor->left == 0 && lprcMonitor->top == 0) {
            return FALSE;
        }
        ++ctx->offset;
    } break;
    case EnumMonitorContext::Action::kMatchMonitorHandle: {
        if (hMonitor == ctx->handle) {
            return FALSE;
        }
        ++ctx->offset;
    } break;
    case EnumMonitorContext::Action::kGetMonitorInfoByOffset: {
        if (ctx->offset == 0) {
            ctx->handle = hMonitor;
            ctx->dc = hdcMonitor;
            ctx->rect = *lprcMonitor;
            ctx->matched = true;
            return FALSE;
        }
        --ctx->offset;
    } break;
    case EnumMonitorContext::Action::kGetMonitorInfoByHandle: {
        if (hMonitor == ctx->handle) {
            ctx->dc = hdcMonitor;
            ctx->rect = *lprcMonitor;
            ctx->matched = true;
            return FALSE;
        }
        ++ctx->offset;
    } break;
    }
    return TRUE;
}

static bool _get_screen_settings_by_index(int screen_index, DEVMODEW &settings) {
    EnumMonitorContext ctx = {EnumMonitorContext::Action::kGetMonitorInfoByOffset, screen_index};
    EnumDisplayMonitors(nullptr, nullptr, _MonitorEnumProc, (LPARAM)&ctx);
    if (!ctx.matched) {
        return false;
    }

    MONITORINFOEXW info;
    info.cbSize = sizeof(info);
    if (!GetMonitorInfoW(ctx.handle, &info)) {
        LOG_ERROR("GetMonitorInfo failed, error=%d\n", GetLastError());
        return false;
    }

    if (!EnumDisplaySettingsW(info.szDevice, ENUM_CURRENT_SETTINGS, &settings)) {
        LOG_ERROR("EnumDisplaySettings(%S) failed, error=%d\n", info.szDevice, GetLastError());
        return false;
    }
    return true;
}

int DisplayServerWindows::get_screen_count() const {
    EnumMonitorContext ctx;
    EnumDisplayMonitors(nullptr, nullptr, _MonitorEnumProc, (LPARAM)&ctx);
    return ctx.offset;
}

int DisplayServerWindows::get_primary_screen_index() const {
    EnumMonitorContext ctx = {EnumMonitorContext::Action::kCheckPrimary};
    EnumDisplayMonitors(nullptr, nullptr, _MonitorEnumProc, (LPARAM)&ctx);
    return ctx.offset;
}

int DisplayServerWindows::get_screen_by_rect(const Rect2 &rc) {
    EnumMonitorContext ctx = {EnumMonitorContext::Action::kMatchMonitorHandle};
    RECT               winRc = {0};
    winRc.left = rc.x;
    winRc.top = rc.y;
    winRc.right = rc.x + rc.width;
    winRc.bottom = rc.y + rc.height;
    ctx.handle = MonitorFromRect(&winRc, MONITOR_DEFAULTTONEAREST);
    EnumDisplayMonitors(nullptr, nullptr, _MonitorEnumProc, (LPARAM)&ctx);
    return ctx.offset;
}

int DisplayServerWindows::get_screen_by_pos(int x, int y) {
    EnumMonitorContext ctx = {EnumMonitorContext::Action::kMatchMonitorHandle};
    POINT              winPoint = {0};
    ctx.handle = MonitorFromPoint(winPoint, MONITOR_DEFAULTTONEAREST);
    EnumDisplayMonitors(nullptr, nullptr, _MonitorEnumProc, (LPARAM)&ctx);
    return ctx.offset;
}

int DisplayServerWindows::get_screen_by_window(uintptr_t window_id) {
    EnumMonitorContext ctx = {EnumMonitorContext::Action::kMatchMonitorHandle};
    ctx.handle = MonitorFromWindow((HWND)window_id, MONITOR_DEFAULTTONEAREST);
    EnumDisplayMonitors(nullptr, nullptr, _MonitorEnumProc, (LPARAM)&ctx);
    return ctx.offset;
}

PlatformDisplayServer::Orientation DisplayServerWindows::get_scren_orientation(int screen_index) const {
    if (screen_index < 0) {
        return Orientation::kUnknown;
    }

    DEVMODEW settings = {0};
    settings.dmFields |= DM_DISPLAYORIENTATION;
    if (!_get_screen_settings_by_index(screen_index, settings)) {
        return Orientation::kUnknown;
    }

    switch (settings.dmDisplayOrientation) {
    case DMDO_DEFAULT: {
        return Orientation::kLandscape;
    } break;
    case DMDO_90: {
        return Orientation::kPortrait;
    } break;
    case DMDO_180: {
        return Orientation::kLandscapeFlipped;
    } break;
    case DMDO_270: {
        return Orientation::kPortraitFlipped;
    } break;
    }
    return Orientation::kUnknown;
}

bool DisplayServerWindows::set_screen_orientation(int screen_index, Orientation orientation) {
    if (screen_index < 0 || orientation == Orientation::kUnknown) {
        return false;
    }

    EnumMonitorContext ctx = {EnumMonitorContext::Action::kGetMonitorInfoByOffset, screen_index};
    EnumDisplayMonitors(nullptr, nullptr, _MonitorEnumProc, (LPARAM)&ctx);
    if (!ctx.matched) {
        return false;
    }

    MONITORINFOEXW info;
    info.cbSize = sizeof(info);
    if (!GetMonitorInfoW(ctx.handle, &info)) {
        LOG_ERROR("GetMonitorInfo failed, error=%d\n", GetLastError());
        return false;
    }

    DEVMODEW settings = {0};
    settings.dmFields |= DM_DISPLAYORIENTATION;
    if (!EnumDisplaySettingsW(info.szDevice, ENUM_CURRENT_SETTINGS, &settings)) {
        LOG_ERROR("EnumDisplaySettings(%S) failed, error=%d\n", info.szDevice, GetLastError());
        return false;
    }

    DWORD newValue = 0;
    switch (orientation) {
    case Orientation::kLandscape:
        newValue = DMDO_DEFAULT;
        break;
    case Orientation::kLandscapeFlipped:
        newValue = DMDO_180;
        break;
    case Orientation::kPortrait:
        newValue = DMDO_90;
        break;
    case Orientation::kPortraitFlipped:
        newValue = DMDO_270;
        break;
    default:
        break;
    }
    if (settings.dmDisplayOrientation == newValue) {
        return true;
    }

    if (!ChangeDisplaySettingsExW(info.szDevice, &settings, NULL, 0, NULL)) {
        LOG_ERROR("ChangeDisplaySettingsExW(%S) failed, error=%d\n", info.szDevice, GetLastError());
        return false;
    }
    return true;
}

std::pair<int, int> DisplayServerWindows::get_screen_size(int screen_index) const {
    EnumMonitorContext ctx = {EnumMonitorContext::Action::kGetMonitorInfoByOffset, screen_index};
    EnumDisplayMonitors(nullptr, nullptr, _MonitorEnumProc, (LPARAM)&ctx);
    if (!ctx.matched) {
        return {};
    }

    return {
        ctx.rect.right - ctx.rect.left, // width
        ctx.rect.bottom - ctx.rect.top  // height
    };
}

Rect2 DisplayServerWindows::get_screen_work_rect(int screen_index) const {
    EnumMonitorContext ctx = {EnumMonitorContext::Action::kGetMonitorInfoByOffset, screen_index};
    EnumDisplayMonitors(nullptr, nullptr, _MonitorEnumProc, (LPARAM)&ctx);
    if (!ctx.matched) {
        return {};
    }

    MONITORINFOEXW info;
    info.cbSize = sizeof(info);
    if (!GetMonitorInfoW(ctx.handle, &info)) {
        LOG_ERROR("GetMonitorInfo failed, error=%d\n", GetLastError());
        return {};
    }

    int   origin = MIN(ctx.rect.left, ctx.rect.top);
    Rect2 result;
    result.x = info.rcWork.left - origin;
    result.y = info.rcWork.top - origin;
    result.width = info.rcWork.right - info.rcWork.left - origin;
    result.height = info.rcWork.bottom - info.rcWork.top - origin;
    return result;
}

int DisplayServerWindows::get_screen_dpi(int screen_index) const {
    EnumMonitorContext ctx = {EnumMonitorContext::Action::kGetMonitorInfoByOffset, screen_index};
    EnumDisplayMonitors(nullptr, nullptr, _MonitorEnumProc, (LPARAM)&ctx);
    if (!ctx.matched) {
        return kDefaultDPI;
    }

    UINT dpiX = 0, dpiY = 0;
    if (GetDpiForMonitor_) {
        if (SUCCEEDED(GetDpiForMonitor_(ctx.handle, MDT_EFFECTIVE_DPI, &dpiX, &dpiY))) {
            return (dpiX + dpiY) / 2;
        }
    }

    // fallback
    dpiX = GetDeviceCaps(ctx.dc, LOGPIXELSX);
    dpiY = GetDeviceCaps(ctx.dc, LOGPIXELSY);
    return (dpiX + dpiY) / 2;
}

float DisplayServerWindows::get_screen_scale_factor(int screen_index) const {
    return float(get_screen_dpi(screen_index)) / kDefaultDPI_F;
}

void DisplayServerWindows::set_window_class_name(const std::string_view cls_name) {
    if (!windowClassRegistered_ && !cls_name.empty()) {
        windowClassName_ = cls_name;
    }
}

std::string DisplayServerWindows::get_window_class_name() const {
    return windowClassName_;
}

uintptr_t DisplayServerWindows::window_create(uintptr_t    parent_window_id,
                                              const Rect2 &rect,
                                              WindowMode   mode,
                                              WindowFlags  flags,
                                              bool         mark_as_primary) {
    if (!windowClassRegistered_) {
        if (!_register_window_class()) {
            return 0;
        }
    }

    return 0;
}

void DisplayServerWindows::window_close(uintptr_t window_id) {}

uintptr_t DisplayServerWindows::window_get_native_handle(uintptr_t window_id) const {
    return 0;
}

void DisplayServerWindows::window_set_primary(uintptr_t window_id) {}

bool DisplayServerWindows::window_is_valid(uintptr_t window_id) const {
    return false;
}

std::string DisplayServerWindows::window_get_class_name(uintptr_t window_id) {
    return {};
}

void DisplayServerWindows::window_show(uintptr_t window_id, WindowMode mode) {}

PlatformDisplayServer::WindowMode DisplayServerWindows::window_get_show_mode(uintptr_t window_id) const {
    return WindowMode::kNormal;
}

bool DisplayServerWindows::window_bring_to_foreground(uintptr_t window_id) {
    return false;
}

bool DisplayServerWindows::window_request_attention(uintptr_t window_id) {
    return false;
}

void DisplayServerWindows::window_popup_at(uintptr_t window_id, int x, int y) {}

PlatformDisplayServer::WindowFlags DisplayServerWindows::window_get_flags(uintptr_t window_id) const {
    return WindowFlags::WINDOW_INVALID_FLAGS;
}

void DisplayServerWindows::window_set_flags(uintptr_t window_id, WindowFlags flags) {}

void DisplayServerWindows::window_set_title(uintptr_t window_id, const std::string_view title) {}

std::string DisplayServerWindows::window_get_title(uintptr_t window_id) const {
    return {};
}

void DisplayServerWindows::window_set_min_size(uintptr_t window_id, int w, int h) {}

std::pair<int, int> DisplayServerWindows::window_get_min_size(uintptr_t window_id) const {
    return {};
}

void DisplayServerWindows::window_set_max_size(uintptr_t window_id, int w, int h) {}

std::pair<int, int> DisplayServerWindows::window_get_max_size(uintptr_t window_id) const {
    return {};
}

void DisplayServerWindows::window_set_size(uintptr_t window_id, int w, int h) {}

std::pair<int, int> DisplayServerWindows::window_get_size(uintptr_t window_id) const {
    return {};
}

std::pair<int, int> DisplayServerWindows::window_get_size_with_decorations(uintptr_t window_id) const {
    return {};
}

void DisplayServerWindows::window_set_pos(uintptr_t window_id, int x, int y) {}

std::pair<int, int> DisplayServerWindows::window_get_pos(uintptr_t window_id) const {
    return {};
}

std::pair<int, int> DisplayServerWindows::window_get_pos_with_decorations(uintptr_t window_id) const {
    return {};
}

void DisplayServerWindows::window_set_current_screen(uintptr_t window_id, int screen_index) {}

void DisplayServerWindows::window_set_ime_enabled(uintptr_t window_id, bool enabled) {}

bool DisplayServerWindows::window_is_ime_enabled(uintptr_t window_id) const {
    return false;
}

void DisplayServerWindows::window_set_ime_position(uintptr_t window_id, int x, int y) {}

bool DisplayServerWindows::window_can_draw(uintptr_t window_id) const {
    return false;
}

}; // namespace mimosa