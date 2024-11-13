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

}; // namespace mimosa