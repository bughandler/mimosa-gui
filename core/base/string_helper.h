#ifndef _STRING_HELPER_H_
#define _STRING_HELPER_H_

#include <string>

namespace mimosa {

std::wstring utf8_to_utf16(const std::string_view s);

std::string utf16_to_utf8(const std::wstring_view s);

#ifdef _WIN32
std::string utf8_to_acp(const std::string_view s);

std::string acp_to_utf8(const std::string_view s);

std::wstring acp_to_utf16(const std::string_view s);

std::string utf16_to_acp(const std::wstring_view s);
#endif

}; // namespace mimosa

#endif // _STRING_HELPER_H_