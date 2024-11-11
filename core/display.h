#ifndef _DISPLAY_SERVER_H_
#define _DISPLAY_SERVER_H_
namespace mimosa {

#include <string>
#include <memory>
#include <vector>
#include <functional>
#include "base/help_defines.h"
#include "type.h"

// The platform-dependent window
class PlatformWindow {
  public:
    enum class Mode {
        kMinimized,
        kNormal,
        kMaximized,
        kFullscreen,
        kExclusiveFullscreen,
    };

    enum class Flags : uint32_t {

    };

    virtual uintptr_t           window_id() const = 0;
    virtual void                show(Mode mode) = 0;
    virtual Mode                get_show_mode() const = 0;
    virtual void                close() = 0;
    virtual void                hide() = 0;
    virtual bool                bring_to_foreground() = 0;
    virtual bool                request_attention() = 0;
    virtual std::string         get_class_name() = 0;
    virtual void                set_title(const std::string_view title) = 0;
    virtual std::string         get_title() const = 0;
    virtual void                set_min_size(int w, int h) = 0;
    virtual std::pair<int, int> get_min_size() const = 0;
    virtual void                set_max_size(int w, int h) = 0;
    virtual std::pair<int, int> get_max_size() const = 0;
    virtual void                set_size(int w, int h) = 0;
    virtual std::pair<int, int> get_size() const = 0;
    virtual std::pair<int, int> get_size_with_decorations() const = 0;
    virtual void                set_pos(int x, int y) = 0;
    virtual std::pair<int, int> get_pos() const = 0;
    virtual std::pair<int, int> get_pos_with_decorations() const = 0;
    virtual int                 get_current_screen_index() const = 0;
    virtual void                set_current_screen(int screen_index) = 0;
    virtual void                set_ime_enabled(bool enabled) = 0;
    virtual bool                is_ime_enabled() const = 0;
    virtual void                set_ime_position(int x, int y) = 0;

    virtual bool can_draw() const = 0;
};

// The abstract layer definitions for platform-dependent functions that related to the following:
//  - Window
//  - Clipboard
//  - IME
//  - Shell/dialogs (e.g. system tray, file selection dialog, toast and so on)
//  - TTS
class PlatformDisplayServer {
    static inline PlatformDisplayServer *instance_;

  public:
    static PlatformDisplayServer *get_singleton();

    PlatformDisplayServer();
    ~PlatformDisplayServer();

    enum class Feature {
        kHIDPI,
        kScreenOrientation,
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
        kTTS
    };

    enum class Orientation {
        kHorizontal,
        kVetical,
    };

    enum class VirtualKeyboardType {
        kDefault,
        kMultiline,
        kNumber,
        kNumberDecimal,
        kPhone,
        kEmailAddress,
        kPassword,
        kUrl
    };

    enum class NativeDialogOptions : std::uint32_t {
        OPT_MULTI_SELECTION,
        OPT_DIR_ONLY,
        OPT_NO_RESOLVE_SYMLINK,
        OPT_NO_CONFIRM_OVERWRITE
    };

    enum class ClipboardDataFormat {
        kUnknown,
        kText,
        kUnicodeText,
        kImage,
        kFileNames,
    };

    struct TTSVoiceInfo {
        std::string name;
        std::string id;
        std::string lang;
    };

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

    using ToastNofiticationCallback = std::function<void(ToastAction, int)>;

    NativeDialogOptions operator|(NativeDialogOptions lhs, NativeDialogOptions rhs) {
        return ENUM_CLASS_OR(NativeDialogOptions, lhs, rhs);
    }
    NativeDialogOptions operator&(NativeDialogOptions lhs, NativeDialogOptions rhs) {
        return ENUM_CLASS_AND(NativeDialogOptions, lhs, rhs);
    }

    virtual bool is_feature_supported(Feature feature);
    virtual bool initialize() = 0;

    // Window creation, reference
    virtual void                            set_window_class_name(const std::string_view cls_name);
    virtual std::string                     get_window_class_name() const;
    virtual std::shared_ptr<PlatformWindow> create_window(uintptr_t parent_window_id, const std::string_view title, PlatformWindow::Flags flags);
    virtual std::shared_ptr<PlatformWindow> ref_window_by_id(uintptr_t window_id) const;
    virtual bool                            is_window_valid(uintptr_t window_id) const;

    // System theme
    virtual bool   is_dark_mode();
    virtual Color3 get_system_accent_color();
    virtual Color3 get_system_base_color();

    // Screen
    virtual int                 get_screen_count() const = 0;
    virtual int                 get_primary_screen_index() const = 0;
    virtual Orientation         get_scren_orientation(int screen_index) const = 0;
    virtual void                set_screen_orientation(int screen_index, Orientation orientation) = 0;
    virtual std::pair<int, int> get_screen_size(int screen_index) const = 0;
    virtual std::pair<int, int> get_screen_work_size(int screen_index) const = 0;
    virtual int                 get_screen_dpi(int screen_index) const = 0;
    virtual int                 get_screen_scale_factor(int screen_index) const = 0;
    virtual int                 get_screen_by_rect(const Rect2 &rc) = 0;
    virtual int                 get_screen_by_window(uintptr_t window_id) = 0;

    // Virtual keyboard
    virtual void virtual_keyboard_show(const std::string_view &existing_text,
                                       const Rect2 &           screen_rect = Rect2(),
                                       VirtualKeyboardType     kb_type = VirtualKeyboardType::kDefault,
                                       int                     max_length = -1,
                                       int                     cursor_start = -1,
                                       int                     cursor_end = -1);
    virtual void virtual_keyboard_hide();
    virtual int  virtual_keyboard_get_height() const;

    // Native dialogs
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

    // Clipboard
    virtual bool                     is_clipboard_has_data() const;
    virtual ClipboardDataFormat      get_clipboard_data_format() const;
    virtual std::string              get_clipboard_text() const;
    virtual std::wstring             get_clipboard_unicode_text() const;
    virtual Image                    get_clipboard_image() const;
    virtual std::vector<std::string> get_clipboard_file_names() const;
    virtual void                     set_clipboard_text(const std::string_view text);
    virtual void                     set_clipboard_unicode_text(const std::wstring_view text);
    virtual void                     set_clipboard_image(const Image &text);

    // TTS
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

    // Toast notification
    virtual bool toast_notification_show(const std::string_view        title,
                                         const std::string_view        text,
                                         const Image &                 image,
                                         ToastPos                      pos,
                                         int                           expire_ms,
                                         bool                          has_audio,
                                         std::vector<std::string_view> actions,
                                         ToastNofiticationCallback &&  handler);
};

} // namespace mimosa
#endif //_DISPLAY_SERVER_H_