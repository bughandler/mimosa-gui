#ifndef _DISPLAY_SERVER_H_
#define _DISPLAY_SERVER_H_

#include <string>

// The abstract layer definitions for platform-dependent functions that related to the following:
//  - Window
//  - Clipboard
//  - IME
//  - TTS
//  - Shell/dialogs (e.g. system tray, file selection dialog, toast and so on)
class DisplayServer {
    static inline DisplayServer *hal_;

  public:
    inline DisplayServer *get_instance() {
        return hal_;
    }

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

    virtual bool is_feature_supported(Feature feature) {
        return false;
    }

    virtual bool initialize() = 0;

    virtual void        set_window_class_name(const std::string_view cls_name) = 0;
    virtual std::string get_window_class_name() = 0;

    virtual uintptr_t create_window(uintptr_t parent_window_id) = 0;
    virtual uintptr_t window_set_mode() = 0;
};

#endif //_DISPLAY_SERVER_H_