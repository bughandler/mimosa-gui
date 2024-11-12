#include "display_windows.h"

namespace mimosa {

bool DisplayServerWindows::initialize() {
    // #TODO: Need further implementation
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

void DisplayServerWindows::set_window_class_name(const std::string_view cls_name) {
    if (!cls_name.empty()) {
        windowClassName_ = cls_name;
    }
}

std::string DisplayServerWindows::get_window_class_name() const {
    return windowClassName_;
}

std::shared_ptr<PlatformWindow> DisplayServerWindows::create_window(uintptr_t              parent_window_id,
                                                                    const std::string_view title,
                                                                    PlatformWindow::Flags  flags) {
    return nullptr;
}

std::shared_ptr<PlatformWindow> DisplayServerWindows::ref_window_by_id(uintptr_t window_id) const {
    return nullptr;
}

bool DisplayServerWindows::is_window_valid(uintptr_t window_id) const {
    return false;
}

}; // namespace mimosa