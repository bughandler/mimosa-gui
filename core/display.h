#ifndef _DISPLAY_SERVER_H_
#define _DISPLAY_SERVER_H_

#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <core/base/help_defines.h>
#include <core/type.h>

namespace mimosa {

// The abstract layer definitions for platform-dependent functions that related to the following:
//  - Screen
//  - Window
//  - Clipboard
//  - Dialogs (e.g. file selection dialog)
//  - System tray icon
//  - Toast notification
//  - IME
//  - TTS
//  - Hot-key
class PlatformDisplayServer {
    static inline PlatformDisplayServer *instance_;

  public:
    static PlatformDisplayServer *get_singleton();

    PlatformDisplayServer();
    ~PlatformDisplayServer();

    enum class Feature {
        kHIDPI,
        kDarkMode,
        kSubWindow,
        kWindowedWindow,
        kFullscreenWindow,
        kBorderlessWindow,
        kExtendToTileWindow,
        kRequestAttention,
        kToastNotificaton,
        kTaskbar,
        kSystemTray,
        kGlobalMenu,
        kNativeFileDialog,
        kNativeDirDialog,
        kVirtualKeyboard,
        kIME,
        kTTS,
        kHotkey,
    };

    virtual bool initialize() = 0;
    virtual bool is_feature_supported(Feature feature) const;

    //
    // System theme
    //

    virtual bool  is_dark_mode() const;
    virtual Color get_system_accent_color() const;
    virtual Color get_system_base_color() const;

    //
    // Screen
    //

    enum class Orientation {
        kUnknown,
        kLandscape,
        kLandscapeFlipped,
        kPortrait,
        kPortraitFlipped,
    };

    virtual int                 get_screen_count() const = 0;
    virtual int                 get_primary_screen_index() const = 0;
    virtual int                 get_screen_by_rect(const Rect2 &rc) = 0;
    virtual int                 get_screen_by_pos(int x, int y) = 0;
    virtual int                 get_screen_by_window(uintptr_t window_id) = 0;
    virtual Orientation         get_scren_orientation(int screen_index) const = 0;
    virtual bool                set_screen_orientation(int screen_index, Orientation orientation) = 0;
    virtual std::pair<int, int> get_screen_size(int screen_index) const = 0;
    virtual Rect2               get_screen_work_rect(int screen_index) const = 0;
    virtual int                 get_screen_dpi(int screen_index) const = 0;
    virtual float               get_screen_scale_factor(int screen_index) const = 0;

    //
    // Window management
    //

    enum class WindowMode {
        kInvalid,
        kMinimized,
        kNormal,
        kMaximized,
        kFullscreen,
        kExclusiveFullscreen,
        kHidden,
    };

    enum class WindowFlags : std::uint32_t {
        WINDOW_INVALID_FLAGS = 0,
        WINDOW_BORDERLESS = 1 << 1,
        WINDOW_CUSTOM_TITLEBAR = 1 << 2,
        WINDOW_POPUP = 1 << 3,
        WINDOW_SPLASH_SCREEN = 1 << 4,
        WINDOW_ALWAYS_ON_TOP = 1 << 5,
        WINDOW_TRANSPARENT = 1 << 6,
        WINDOW_NO_FOCUS = 1 << 7,
        WINDOW_NO_RESIZABLE = 1 << 8,
        WINDOW_PASSTRHOUGH = 1 << 9,
    };
    DECL_ENUM_CLASS_BITOPS(WindowFlags)

    virtual void                set_window_class_name(const std::string_view cls_name);
    virtual std::string         get_window_class_name() const;
    virtual uintptr_t           window_create(uintptr_t    parent_window_id,
                                              const Rect2 &rect,
                                              WindowMode   mode,
                                              WindowFlags  flags,
                                              bool         mark_as_primary);
    virtual void                window_close(uintptr_t window_id);
    virtual uintptr_t           window_get_native_handle(uintptr_t window_id) const = 0;
    virtual void                window_set_primary(uintptr_t window_id) = 0;
    virtual bool                window_is_valid(uintptr_t window_id) const;
    virtual std::string         window_get_class_name(uintptr_t window_id);
    virtual void                window_show(uintptr_t window_id, WindowMode mode);
    virtual WindowMode          window_get_show_mode(uintptr_t window_id) const;
    virtual bool                window_bring_to_foreground(uintptr_t window_id);
    virtual bool                window_request_attention(uintptr_t window_id);
    virtual void                window_popup_at(uintptr_t window_id, int x, int y);
    virtual WindowFlags         window_get_flags(uintptr_t window_id) const;
    virtual void                window_set_flags(uintptr_t window_id, WindowFlags flags);
    virtual void                window_set_title(uintptr_t window_id, const std::string_view title);
    virtual std::string         window_get_title(uintptr_t window_id) const;
    virtual void                window_set_min_size(uintptr_t window_id, int w, int h);
    virtual std::pair<int, int> window_get_min_size(uintptr_t window_id) const;
    virtual void                window_set_max_size(uintptr_t window_id, int w, int h);
    virtual std::pair<int, int> window_get_max_size(uintptr_t window_id) const;
    virtual void                window_set_size(uintptr_t window_id, int w, int h);
    virtual std::pair<int, int> window_get_size(uintptr_t window_id) const;
    virtual std::pair<int, int> window_get_size_with_decorations(uintptr_t window_id) const;
    virtual void                window_set_pos(uintptr_t window_id, int x, int y);
    virtual std::pair<int, int> window_get_pos(uintptr_t window_id) const;
    virtual std::pair<int, int> window_get_pos_with_decorations(uintptr_t window_id) const;
    virtual void                window_set_current_screen(uintptr_t window_id, int screen_index) = 0;
    virtual void                window_set_ime_enabled(uintptr_t window_id, bool enabled) = 0;
    virtual bool                window_is_ime_enabled(uintptr_t window_id) const = 0;
    virtual void                window_set_ime_position(uintptr_t window_id, int x, int y) = 0;
    virtual bool                window_can_draw(uintptr_t window_id) const = 0;

    //
    // Virtual keyboard
    //

    enum class VirtualKeyboardType {
        kDefault,
        kMultiline,
        kNumber,
        kNumberDecimal,
        kPhone,
        kEmailAddress,
        kPassword,
        kUrl,
    };

    virtual void virtual_keyboard_show(const std::string_view &existing_text,
                                       const Rect2 &           screen_rect = Rect2(),
                                       VirtualKeyboardType     kb_type = VirtualKeyboardType::kDefault,
                                       int                     max_length = -1,
                                       int                     cursor_start = -1,
                                       int                     cursor_end = -1);
    virtual void virtual_keyboard_hide();
    virtual int  virtual_keyboard_get_height() const;

    //
    // Native dialogs
    //

    enum class NativeDialogOptions : std::uint32_t {
        OPT_MULTI_SELECTION = 1 << 1,
        OPT_DIR_ONLY = 1 << 2,
        OPT_NO_RESOLVE_SYMLINK = 1 << 3,
        OPT_NO_CONFIRM_OVERWRITE = 1 << 4
    };
    DECL_ENUM_CLASS_BITOPS(NativeDialogOptions)

    virtual std::vector<std::string> get_open_file_names(uintptr_t              parent_window_id,
                                                         const std::string_view caption,
                                                         const std::string_view root,
                                                         const std::string_view filters,
                                                         NativeDialogOptions    options);
    virtual std::string              get_save_file_name(uintptr_t              parent_window_id,
                                                        const std::string_view caption,
                                                        const std::string_view root,
                                                        const std::string_view filters,
                                                        NativeDialogOptions    options);
    virtual std::string              get_existing_directory(uintptr_t              parent_window_id,
                                                            const std::string_view caption,
                                                            const std::string_view root,
                                                            NativeDialogOptions    options);

    //
    // Clipboard
    //

    enum class ClipboardDataFormat {
        kUnknown,
        kText,
        kUnicodeText,
        kImage,
        kFileNames,
    };

    virtual bool                     is_clipboard_has_data() const;
    virtual ClipboardDataFormat      get_clipboard_data_format() const;
    virtual std::string              get_clipboard_text() const;
    virtual std::wstring             get_clipboard_unicode_text() const;
    virtual Image                    get_clipboard_image() const;
    virtual std::vector<std::string> get_clipboard_file_names() const;
    virtual bool                     set_clipboard_text(const std::string_view text);
    virtual bool                     set_clipboard_unicode_text(const std::wstring_view text);
    virtual bool                     set_clipboard_image(const Image &text);

    //
    // TTS
    //

    struct TTSVoiceInfo {
        std::string name;
        std::string id;
        std::string lang;
    };

    virtual std::vector<TTSVoiceInfo> tts_get_voices() const;
    virtual std::string               tts_get_voice_id_from_lang(const std::string_view lang_name) const;
    virtual bool                      tts_is_speaking() const;
    virtual bool                      tts_is_paused() const;
    virtual void                      tts_speak(const std::string_view &text,
                                                const std::string_view &voice_id,
                                                int                     volume = 50,
                                                float                   pitch = 1.f,
                                                float                   rate = 1.f,
                                                int                     utterance_id = 0,
                                                bool                    interrupt = false);
    virtual void                      tts_pause();
    virtual void                      tts_resume();
    virtual void                      tts_stop();

    //
    // Toast notification
    //

    enum class ToastPos {
        kDefault,
        kCenter,
        kBottom,
        kTop,
    };

    enum class ToastAction {
        kActivate,
        kDismiss,
    };

    // (action, action-index)
    using ToastNofiticationCallback = std::function<void(ToastAction, int)>;

    virtual bool toast_notification_show(const std::string_view        title,
                                         const std::string_view        text,
                                         const Image &                 image,
                                         ToastPos                      pos,
                                         int                           timeout_ms,
                                         bool                          has_audio,
                                         std::vector<std::string_view> actions,
                                         ToastNofiticationCallback &&  handler);

    //
    // System tray icon
    //

    enum class SystemIcon {
        kNone,
        kInformation,
        kWarning,
        kCirtical,
    };

    enum class ActivateReason {
        kClick,
        kDoubleClick,
        kMiddleClick,
        kContextClick,
    };

    using TrayIconCallback = std::function<void(ActivateReason, Position2)>;

    virtual uintptr_t   trayicon_create(const Image &icon, const std::string_view tooltip, TrayIconCallback &&handler);
    virtual bool        trayicon_dismiss(uintptr_t tray_id);
    virtual void        trayicon_show(uintptr_t tray_id, bool show);
    virtual bool        trayicon_is_visible(uintptr_t tray_id);
    virtual Rect2       trayicon_get_rect() const;
    virtual Image       trayicon_get_icon() const;
    virtual void        trayicon_set_icon(const Image &icon);
    virtual std::string trayicon_get_tooltip() const;
    virtual void        trayicon_set_tooltip(const std::string_view tooltip);
    virtual void trayicon_pop_message(const std::string_view title, const std::string_view message, int timeout_ms);
    virtual void trayicon_pop_message_icon(const std::string_view title,
                                           const std::string_view message,
                                           const Image &          icon,
                                           int                    timeout_ms);
    virtual void trayicon_pop_message_icon(const std::string_view title,
                                           const std::string_view message,
                                           SystemIcon             icon,
                                           int                    timeout_ms);

    //
    // Hot-key
    //

    enum class HotkeyModifier {
        KEY_NONE = 0,
        KEY_CTRL = 1 << 1,
        KEY_ALT = 1 << 2,
        KEY_SHIFT = 1 << 3,
        KEY_WIN = 1 << 4,
        KEY_CMD = 1 << 5,
    };
    DECL_ENUM_CLASS_BITOPS(HotkeyModifier)

    // (hot-key id, modifiers, key code)
    using HotkeyCallback = std::function<void(int, HotkeyModifier, int)>;

    virtual int  hotkey_create_id();
    virtual bool hotkey_register(int id, HotkeyModifier modifiers, int key_code, HotkeyCallback &&handler);
    virtual bool hotkey_remove(int id);
};

} // namespace mimosa
#endif //_DISPLAY_SERVER_H_