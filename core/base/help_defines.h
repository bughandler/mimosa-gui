#ifndef _CORE_DEFINES_H_
#define _CORE_DEFINES_H_

#include <type_traits>

namespace mimosa {

// Turn argument to string constant:
// https://gcc.gnu.org/onlinedocs/cpp/Stringizing.html#Stringizing
#ifndef _STR
#define _STR(m_x)   #m_x
#define _MKSTR(m_x) _STR(m_x)
#endif

// Should always inline no matter what.
#ifndef _ALWAYS_INLINE_
#if defined(__GNUC__)
#define _ALWAYS_INLINE_ __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
#define _ALWAYS_INLINE_ __forceinline
#else
#define _ALWAYS_INLINE_ inline
#endif
#endif

// Should always inline, except in dev builds because it makes debugging harder.
#ifndef _FORCE_INLINE_
#ifdef DEV_ENABLED
#define _FORCE_INLINE_ inline
#else
#define _FORCE_INLINE_ _ALWAYS_INLINE_
#endif
#endif

// In some cases [[nodiscard]] will get false positives,
// we can prevent the warning in specific cases by preceding the call with a cast.
#ifndef _ALLOW_DISCARD_
#define _ALLOW_DISCARD_ (void)
#endif

#if defined(__GNUC__)
#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x)   x
#define unlikely(x) x
#endif

#define DECL_ENUM_CLASS_BITOPS(ENUM_CLS)                                                   \
    friend ENUM_CLS operator|(ENUM_CLS lhs, ENUM_CLS rhs) {                                \
        return static_cast<ENUM_CLS>(static_cast<std::underlying_type_t<ENUM_CLS>>(lhs) |  \
                                     static_cast<std::underlying_type_t<ENUM_CLS>>(rhs));  \
    }                                                                                      \
    friend ENUM_CLS operator&(ENUM_CLS lhs, ENUM_CLS rhs) {                                \
        return static_cast<ENUM_CLS>(static_cast<std::underlying_type_t<ENUM_CLS>>(lhs) &  \
                                     static_cast<std::underlying_type_t<ENUM_CLS>>(rhs));  \
    }                                                                                      \
    friend ENUM_CLS operator^(ENUM_CLS lhs, ENUM_CLS rhs) {                                \
        return static_cast<ENUM_CLS>(static_cast<std::underlying_type_t<ENUM_CLS>>(lhs) ^  \
                                     static_cast<std::underlying_type_t<ENUM_CLS>>(rhs));  \
    }                                                                                      \
    friend ENUM_CLS operator~(ENUM_CLS lhs) {                                              \
        return static_cast<ENUM_CLS>(~static_cast<std::underlying_type_t<ENUM_CLS>>(lhs)); \
    }

} // namespace mimosa
#endif //_CORE_DEFINES_H_