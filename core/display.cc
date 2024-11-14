#include <core/base/log.h>
#include "display.h"

namespace mimosa {

PlatformDisplayServer *PlatformDisplayServer::get_singleton() {
    return instance_;
}

PlatformDisplayServer::PlatformDisplayServer() {
    // Assign as singleton
    instance_ = this;
}

PlatformDisplayServer::~PlatformDisplayServer() {
    instance_ = nullptr;
}

bool PlatformDisplayServer::is_feature_supported(Feature feature) const {
    return false;
}

void PlatformDisplayServer::set_window_class_name(const std::string_view cls_name) {
    LOG_WARNING("This display server doesn't support window class name.");
}

std::string PlatformDisplayServer::get_window_class_name() const {
    LOG_WARNING("This display server doesn't support window class name.");
    return {};
}

uintptr_t PlatformDisplayServer::window_create(uintptr_t    parent_window_id,
                                               const Rect2 &rect,
                                               WindowMode   mode,
                                               WindowFlags  flags,
                                               bool         mark_as_primary) {
    LOG_WARNING("This display server doesn't support window creation.");
    return {};
}

void PlatformDisplayServer::window_close(uintptr_t window_id) {
    LOG_WARNING("This display server doesn't support window closing.");
}

bool PlatformDisplayServer::window_is_valid(uintptr_t window_id) const {
    LOG_WARNING("This display server doesn't support window checking.");
    return false;
}

std::string PlatformDisplayServer::window_get_class_name(uintptr_t window_id) {
    LOG_WARNING("This display server doesn't support window class name.");
    return {};
}

void PlatformDisplayServer::window_show(uintptr_t window_id, WindowMode mode) {
    LOG_WARNING("This display server doesn't support window mode.");
}

PlatformDisplayServer::WindowMode PlatformDisplayServer::window_get_show_mode(uintptr_t window_id) const {
    LOG_WARNING("This display server doesn't support window mode.");
    return WindowMode::kInvalid;
}

bool PlatformDisplayServer::window_bring_to_foreground(uintptr_t window_id) {
    LOG_WARNING("This display server doesn't support bring window to foreground.");
    return false;
}

bool PlatformDisplayServer::window_request_attention(uintptr_t window_id) {
    LOG_WARNING("This display server doesn't support window attenation.");
    return false;
}

void PlatformDisplayServer::window_popup_at(uintptr_t window_id, int x, int y) {
    LOG_WARNING("This display server doesn't support window popup.");
}

PlatformDisplayServer::WindowFlags PlatformDisplayServer::window_get_flags(uintptr_t window_id) const {
    LOG_WARNING("This display server doesn't support window flags.");
    return WindowFlags::WINDOW_INVALID_FLAGS;
}

void PlatformDisplayServer::window_set_flags(uintptr_t window_id, WindowFlags flags) {
    LOG_WARNING("This display server doesn't support window flags.");
}

void PlatformDisplayServer::window_set_title(uintptr_t window_id, const std::string_view title) {
    LOG_WARNING("This display server doesn't support window title.");
}

std::string PlatformDisplayServer::window_get_title(uintptr_t window_id) const {
    LOG_WARNING("This display server doesn't support window title.");
    return {};
}

void PlatformDisplayServer::window_set_min_size(uintptr_t window_id, int w, int h) {
    LOG_WARNING("This display server doesn't support window min-size.");
}

std::pair<int, int> PlatformDisplayServer::window_get_min_size(uintptr_t window_id) const {
    LOG_WARNING("This display server doesn't support window min-size.");
    return {0, 0};
}

void PlatformDisplayServer::window_set_max_size(uintptr_t window_id, int w, int h) {
    LOG_WARNING("This display server doesn't support window max-size.");
}

std::pair<int, int> PlatformDisplayServer::window_get_max_size(uintptr_t window_id) const {
    LOG_WARNING("This display server doesn't support window max-size.");
    return {0, 0};
}

void PlatformDisplayServer::window_set_size(uintptr_t window_id, int w, int h) {
    LOG_WARNING("This display server doesn't support window size.");
}

std::pair<int, int> PlatformDisplayServer::window_get_size(uintptr_t window_id) const {
    LOG_WARNING("This display server doesn't support window size.");
    return {0, 0};
}

std::pair<int, int> PlatformDisplayServer::window_get_size_with_decorations(uintptr_t window_id) const {
    LOG_WARNING("This display server doesn't support window size.");
    return {0, 0};
}

void PlatformDisplayServer::window_set_pos(uintptr_t window_id, int x, int y) {
    LOG_WARNING("This display server doesn't support window position.");
}

std::pair<int, int> PlatformDisplayServer::window_get_pos(uintptr_t window_id) const {
    LOG_WARNING("This display server doesn't support window position.");
    return {0, 0};
}

std::pair<int, int> PlatformDisplayServer::window_get_pos_with_decorations(uintptr_t window_id) const {
    LOG_WARNING("This display server doesn't support window position.");
    return {0, 0};
}

void PlatformDisplayServer::window_set_ime_enabled(uintptr_t window_id, bool enabled) {
    LOG_WARNING("This display server doesn't support IME for window.");
}

bool PlatformDisplayServer::window_is_ime_enabled(uintptr_t window_id) const {
    LOG_WARNING("This display server doesn't support IME for window.");
    return false;
}

void PlatformDisplayServer::window_set_ime_position(uintptr_t window_id, int x, int y) {
    LOG_WARNING("This display server doesn't support IME position.");
}

bool PlatformDisplayServer::is_dark_mode() const {
    LOG_WARNING("This display server doesn't support dark mode.");
    return false;
}

Color PlatformDisplayServer::get_system_accent_color() const {
    return {};
}

Color PlatformDisplayServer::get_system_base_color() const {
    return {};
}

void PlatformDisplayServer::virtual_keyboard_show(const std::string_view &existing_text,
                                                  const Rect2 &           screen_rect /*= Rect2()*/,
                                                  VirtualKeyboardType     kb_type /*= VirtualKeyboardType::kDefault*/,
                                                  int                     max_length /*= -1*/,
                                                  int                     cursor_start /*= -1*/,
                                                  int                     cursor_end /*= -1*/) {
    LOG_WARNING("This display server doesn't support virtual keyboard.");
}

void PlatformDisplayServer::virtual_keyboard_hide() {
    LOG_WARNING("This display server doesn't support virtual keyboard.");
}

int PlatformDisplayServer::virtual_keyboard_get_height() const {
    LOG_WARNING("This display server doesn't support virtual keyboard.");
    return 0;
}

std::vector<std::string> PlatformDisplayServer::get_open_file_names(uintptr_t              parent_window_id,
                                                                    const std::string_view caption,
                                                                    const std::string_view root,
                                                                    const std::string_view filters,
                                                                    NativeDialogOptions    options) {
    LOG_WARNING("This display server doesn't support native dialog for opening files.");
    return {};
}

std::string PlatformDisplayServer::get_save_file_name(uintptr_t              parent_window_id,
                                                      const std::string_view caption,
                                                      const std::string_view root,
                                                      const std::string_view filters,
                                                      NativeDialogOptions    options) {
    LOG_WARNING("This display server doesn't support native dialog for saving file.");
    return {};
}

std::string PlatformDisplayServer::get_existing_directory(uintptr_t              parent_window_id,
                                                          const std::string_view caption,
                                                          const std::string_view root,
                                                          NativeDialogOptions    options) {
    LOG_WARNING("This display server doesn't support native dialog for selecting directory.");
    return {};
}

bool PlatformDisplayServer::is_clipboard_has_data() const {
    LOG_WARNING("This display server doesn't support clipboard.");
    return false;
}

PlatformDisplayServer::ClipboardDataFormat PlatformDisplayServer::get_clipboard_data_format() const {
    LOG_WARNING("This display server doesn't support clipboard format detection.");
    return ClipboardDataFormat::kUnknown;
}

std::string PlatformDisplayServer::get_clipboard_text() const {
    LOG_WARNING("This display server doesn't support clipboard for text.");
    return {};
}

std::wstring PlatformDisplayServer::get_clipboard_unicode_text() const {
    LOG_WARNING("This display server doesn't support clipboard for unicode text.");
    return {};
}

Image PlatformDisplayServer::get_clipboard_image() const {
    LOG_WARNING("This display server doesn't support clipboard for image.");
    return {};
}

std::vector<std::string> PlatformDisplayServer::get_clipboard_file_names() const {
    LOG_WARNING("This display server doesn't support clipboard for files.");
    return {};
}

bool PlatformDisplayServer::set_clipboard_text(const std::string_view text) {
    LOG_WARNING("This display server doesn't support writting clipboard for text.");
    return false;
}

bool PlatformDisplayServer::set_clipboard_unicode_text(const std::wstring_view text) {
    LOG_WARNING("This display server doesn't support writting clipboard for unicode text.");
    return false;
}

bool PlatformDisplayServer::set_clipboard_image(const Image &text) {
    LOG_WARNING("This display server doesn't support writting clipboard for image.");
    return false;
}

std::vector<mimosa::PlatformDisplayServer::TTSVoiceInfo> PlatformDisplayServer::tts_get_voices() const {
    LOG_WARNING("This display server doesn't support TTS.");
    return {};
}

std::string PlatformDisplayServer::tts_get_voice_id_from_lang(const std::string_view lang_name) const {
    LOG_WARNING("This display server doesn't support TTS.");
    return {};
}

bool PlatformDisplayServer::tts_is_speaking() const {
    LOG_WARNING("This display server doesn't support TTS.");
    return false;
}

bool PlatformDisplayServer::tts_is_paused() const {
    LOG_WARNING("This display server doesn't support TTS.");
    return false;
}

void PlatformDisplayServer::tts_speak(const std::string_view &text,
                                      const std::string_view &voice_id,
                                      int                     volume /*= 50*/,
                                      float                   pitch /*= 1.f*/,
                                      float                   rate /*= 1.f*/,
                                      int                     utterance_id /*= 0*/,
                                      bool                    interrupt /*= false*/) {
    LOG_WARNING("This display server doesn't support TTS.");
}

void PlatformDisplayServer::tts_pause() {
    LOG_WARNING("This display server doesn't support TTS.");
}

void PlatformDisplayServer::tts_resume() {
    LOG_WARNING("This display server doesn't support TTS.");
}

void PlatformDisplayServer::tts_stop() {
    LOG_WARNING("This display server doesn't support TTS.");
}

bool PlatformDisplayServer::toast_notification_show(const std::string_view        title,
                                                    const std::string_view        text,
                                                    const Image &                 image,
                                                    ToastPos                      pos,
                                                    int                           timeout_ms,
                                                    bool                          has_audio,
                                                    std::vector<std::string_view> actions,
                                                    ToastNofiticationCallback &&  handler) {
    LOG_WARNING("This display server doesn't support toast notification.");
    return false;
}

uintptr_t
PlatformDisplayServer::trayicon_create(const Image &icon, const std::string_view tooltip, TrayIconCallback &&handler) {
    LOG_WARNING("This display server doesn't support tray-icon.");
    return 0;
}

bool PlatformDisplayServer::trayicon_dismiss(uintptr_t tray_id) {
    LOG_WARNING("This display server doesn't support tray-icon.");
    return false;
}

void PlatformDisplayServer::trayicon_show(uintptr_t tray_id, bool show) {
    LOG_WARNING("This display server doesn't support tray-icon.");
}

bool PlatformDisplayServer::trayicon_is_visible(uintptr_t tray_id) {
    LOG_WARNING("This display server doesn't support tray-icon.");
    return false;
}

Rect2 PlatformDisplayServer::trayicon_get_rect() const {
    LOG_WARNING("This display server doesn't support tray-icon.");
    return {};
}

Image PlatformDisplayServer::trayicon_get_icon() const {
    LOG_WARNING("This display server doesn't support tray-icon.");
    return {};
}

void PlatformDisplayServer::trayicon_set_icon(const Image &icon) {
    LOG_WARNING("This display server doesn't support tray-icon.");
}

std::string PlatformDisplayServer::trayicon_get_tooltip() const {
    LOG_WARNING("This display server doesn't support tray-icon.");
    return {};
}

void PlatformDisplayServer::trayicon_set_tooltip(const std::string_view tooltip) {
    LOG_WARNING("This display server doesn't support tray-icon.");
}

void PlatformDisplayServer::trayicon_pop_message(const std::string_view title,
                                                 const std::string_view message,
                                                 int                    timeout_ms) {
    LOG_WARNING("This display server doesn't support tray-icon.");
}

void PlatformDisplayServer::trayicon_pop_message_icon(const std::string_view title,
                                                      const std::string_view message,
                                                      const Image &          icon,
                                                      int                    timeout_ms) {
    LOG_WARNING("This display server doesn't support tray-icon.");
}

void PlatformDisplayServer::trayicon_pop_message_icon(const std::string_view title,
                                                      const std::string_view message,
                                                      SystemIcon             icon,
                                                      int                    timeout_ms) {
    LOG_WARNING("This display server doesn't support tray-icon.");
}

int PlatformDisplayServer::hotkey_create_id() {
    LOG_WARNING("This display server doesn't support hot-key id generation.");
    return false;
}

bool PlatformDisplayServer::hotkey_register(int id, HotkeyModifier modifiers, int key_code, HotkeyCallback &&handler) {
    LOG_WARNING("This display server doesn't support hot-key registration.");
    return false;
}

bool PlatformDisplayServer::hotkey_remove(int id) {
    LOG_WARNING("This display server doesn't support hot-key uninstallation.");
    return false;
}

}; // namespace mimosa