#ifndef _CORE_DEFINES_H_
#define _CORE_DEFINES_H_

#define ENUM_CLASS_OR(t, l, r) \
    static_cast<t>(static_cast<std::underlying_type_t>(l) | static_cast<std::underlying_type_t>(r));
#define ENUM_CLASS_AND(t, l, r) \
    static_cast<t>(static_cast<std::underlying_type_t>(l) & static_cast<std::underlying_type_t>(r));

#endif //_CORE_DEFINES_H_