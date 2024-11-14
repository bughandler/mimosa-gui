#ifdef _WIN32
#include <windows.h>
#else
#include <string.h>
#include <locale>
#include <codecvt>
#endif
#include "string_helper.h"

namespace mimosa {

#ifdef _WIN32
std::string u2a(uint32_t codePage, const std::wstring_view s) {
    int len = WideCharToMultiByte(codePage, 0, s.data(), s.size(), nullptr, 0, nullptr, nullptr);
    if (!len) {
        return {};
    }

    std::string result;
    result.resize(len);
    if (result.empty()) {
        return {};
    }

    if (WideCharToMultiByte(codePage, 0, s.data(), s.size(), &result[0], len, nullptr, nullptr) != len) {
        return {};
    }

    return result;
}

std::wstring a2u(uint32_t codePage, const std::string_view s) {
    int len = MultiByteToWideChar(codePage, 0, s.data(), s.size(), nullptr, 0);
    if (!len) {
        return {};
    }

    std::wstring result;
    result.resize(len);
    if (result.empty()) {
        return {};
    }

    if (MultiByteToWideChar(codePage, 0, s.data(), s.size(), (LPWSTR)&result[0], len) != len) {
        return {};
    }

    return result;
}
#endif

std::wstring utf8_to_utf16(const std::string_view s) {
    if (s.empty()) {
        return {};
    }
#ifdef _WIN32
    return a2u(CP_UTF8, s);
#else
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    return conv.from_bytes(s.data(), s.data() + s.size());
#endif
}

std::string utf16_to_utf8(const std::wstring_view s) {
    if (s.empty()) {
        return {};
    }
#ifdef _WIN32
    return u2a(CP_UTF8, s);
#else
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    return conv.to_bytes(s.data(), s.data() + s.size());
#endif
}

}; // namespace mimosa