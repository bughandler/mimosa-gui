#ifdef _WIN32
#ifndef _DISPLAY_IMPL_WINDOWS_H_
#define _DISPLAY_IMPL_WINDOWS_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <core/display.h>

namespace mimosa {

class DisplayServerWindows : public PlatformDisplayServer {
    std::string windowClassName_ = "mimosa_window";

  public:
    virtual bool initialize() override;
    virtual bool is_feature_supported(Feature feature) const override;

    virtual void        set_window_class_name(const std::string_view cls_name) override;
    virtual std::string get_window_class_name() const override;
    virtual std::shared_ptr<PlatformWindow>
    create_window(uintptr_t parent_window_id, const std::string_view title, PlatformWindow::Flags flags) override;
    virtual std::shared_ptr<PlatformWindow> ref_window_by_id(uintptr_t window_id) const override;
    virtual bool                            is_window_valid(uintptr_t window_id) const override;
};

};     // namespace mimosa
#endif //_DISPLAY_IMPL_WINDOWS_H_
#endif // _WIN32