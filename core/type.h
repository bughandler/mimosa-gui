#ifndef _CORE_TYPE_H_
#define _CORE_TYPE_H_
namespace mimosa {

#include <vector>

struct Position2 {
    int x = 0, y = 0;
};

struct Rect2 {
    int x = 0, y = 0;
    int width = 0, height = 0;
};

struct Color {
    int r = 0, g = 0, b = 0, a = 0;
};

struct Image {
    enum class Format {
        // RGB (no alpha channel)
        kRGB8,
        // RGBA (8bits)
        kRGBA8,
    };
    Format               format = Format::kRGB8;
    int                  width = 0;
    int                  height = 0;
    std::vector<uint8_t> data;
};

} // namespace mimosa
#endif //_CORE_TYPE_H_