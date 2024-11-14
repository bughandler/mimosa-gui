#ifdef _WIN32
#ifndef _DISPLAY_IMPL_WINDOWS_H_
#define _DISPLAY_IMPL_WINDOWS_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <core/display.h>

namespace mimosa {

class DisplayServerWindows : public PlatformDisplayServer {
    std::string windowClassName_ = "mimosa_window";
    bool        windowClassRegistered_ = false;

    friend static LRESULT CALLBACK _WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT                        WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    bool _register_window_class();

  public:
    virtual bool initialize() override;
    virtual bool is_feature_supported(Feature feature) const override;

    virtual bool  is_dark_mode() const override;
    virtual Color get_system_accent_color() const override;
    virtual Color get_system_base_color() const override;

    virtual int                 get_screen_count() const override;
    virtual int                 get_primary_screen_index() const override;
    virtual int                 get_screen_by_rect(const Rect2 &rc) override;
    virtual int                 get_screen_by_pos(int x, int y) override;
    virtual int                 get_screen_by_window(uintptr_t window_id) override;
    virtual Orientation         get_scren_orientation(int screen_index) const override;
    virtual bool                set_screen_orientation(int screen_index, Orientation orientation) override;
    virtual std::pair<int, int> get_screen_size(int screen_index) const override;
    virtual Rect2               get_screen_work_rect(int screen_index) const override;
    virtual int                 get_screen_dpi(int screen_index) const override;
    virtual float               get_screen_scale_factor(int screen_index) const override;

    virtual void                set_window_class_name(const std::string_view cls_name) override;
    virtual std::string         get_window_class_name() const override;
    virtual uintptr_t           window_create(uintptr_t    parent_window_id,
                                              const Rect2 &rect,
                                              WindowMode   mode,
                                              WindowFlags  flags,
                                              bool         mark_as_primary) override;
    virtual void                window_close(uintptr_t window_id);
    virtual uintptr_t           window_get_native_handle(uintptr_t window_id) const override;
    virtual void                window_set_primary(uintptr_t window_id) override;
    virtual bool                window_is_valid(uintptr_t window_id) const override;
    virtual std::string         window_get_class_name(uintptr_t window_id) override;
    virtual void                window_show(uintptr_t window_id, WindowMode mode) override;
    virtual WindowMode          window_get_show_mode(uintptr_t window_id) const override;
    virtual bool                window_bring_to_foreground(uintptr_t window_id) override;
    virtual bool                window_request_attention(uintptr_t window_id) override;
    virtual void                window_popup_at(uintptr_t window_id, int x, int y) override;
    virtual WindowFlags         window_get_flags(uintptr_t window_id) const override;
    virtual void                window_set_flags(uintptr_t window_id, WindowFlags flags) override;
    virtual void                window_set_title(uintptr_t window_id, const std::string_view title) override;
    virtual std::string         window_get_title(uintptr_t window_id) const override;
    virtual void                window_set_min_size(uintptr_t window_id, int w, int h) override;
    virtual std::pair<int, int> window_get_min_size(uintptr_t window_id) const override;
    virtual void                window_set_max_size(uintptr_t window_id, int w, int h) override;
    virtual std::pair<int, int> window_get_max_size(uintptr_t window_id) const override;
    virtual void                window_set_size(uintptr_t window_id, int w, int h) override;
    virtual std::pair<int, int> window_get_size(uintptr_t window_id) const override;
    virtual std::pair<int, int> window_get_size_with_decorations(uintptr_t window_id) const override;
    virtual void                window_set_pos(uintptr_t window_id, int x, int y) override;
    virtual std::pair<int, int> window_get_pos(uintptr_t window_id) const override;
    virtual std::pair<int, int> window_get_pos_with_decorations(uintptr_t window_id) const override;
    virtual void                window_set_current_screen(uintptr_t window_id, int screen_index) override;
    virtual void                window_set_ime_enabled(uintptr_t window_id, bool enabled) override;
    virtual bool                window_is_ime_enabled(uintptr_t window_id) const override;
    virtual void                window_set_ime_position(uintptr_t window_id, int x, int y) override;
    virtual bool                window_can_draw(uintptr_t window_id) const override;
};

};     // namespace mimosa
#endif //_DISPLAY_IMPL_WINDOWS_H_
#endif // _WIN32