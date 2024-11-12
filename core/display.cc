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

std::shared_ptr<PlatformWindow> PlatformDisplayServer::create_window(uintptr_t              parent_window_id,
                                                                     const std::string_view title,
                                                                     PlatformWindow::Flags  flags) {
    LOG_WARNING("This display server doesn't support window creation.");
    return nullptr;
}

std::shared_ptr<mimosa::PlatformWindow> PlatformDisplayServer::ref_window_by_id(uintptr_t window_id) const {
    LOG_WARNING("This display server doesn't support window manager.");
    return nullptr;
}

bool PlatformDisplayServer::is_window_valid(uintptr_t window_id) const {
    LOG_WARNING("This display server doesn't support window checking.");
    return false;
}

bool PlatformDisplayServer::is_dark_mode() {
    LOG_WARNING("This display server doesn't support dark mode.");
    return false;
}

Color3 PlatformDisplayServer::get_system_accent_color() {
    return {};
}

Color3 PlatformDisplayServer::get_system_base_color() {
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

std::shared_ptr<mimosa::PlatformTrayIcon> PlatformDisplayServer::create_system_tray_icon(const Image &          icon,
                                                                                         const std::string_view hint) {
    LOG_WARNING("This display server doesn't support system tray icon.");
    return nullptr;
}

}; // namespace mimosa