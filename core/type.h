#ifndef _CORE_TYPE_H_
#define _CORE_TYPE_H_

#include <vector>

struct Rect2 {
    int x = 0, y = 0;
    int width = 0, height = 0;
};

struct Color3 {
    int r = 0, g = 0, b = 0;
};

struct SimpleImage {
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

#endif //_CORE_TYPE_H_