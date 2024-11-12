#ifndef _BASE_LOG_H_
#define _BASE_LOG_H_

namespace mimosa {

// Log devices
#define LOG_DEVICE_NONE   0
#define LOG_DEVICE_STDERR 1
#ifdef _WIN32
#define LOG_DEVICE_DEBUG_PORT 2
#endif //_WIN32
#define LOG_DEVICE_FILE 3

// Log levels
#define LOG_LEVEL_DEBUG    1
#define LOG_LEVEL_INFO     2
#define LOG_LEVEL_WARNING  3
#define LOG_LEVEL_ERROR    4
#define LOG_LEVEL_DISABLED 99

#ifdef NO_LOG
// Disable logging
#define LOG_FILTER_LEVEL  LOG_LEVEL_DISABLED
#define LOG_OUTPUT_DEVICE LOG_DEVICE_NONE
#else
// Default log settings

// Default log level
#ifndef LOG_FILTER_LEVEL
#ifdef NDEBUG
#define LOG_FILTER_LEVEL LOG_LEVEL_INFO
#else
#define LOG_FILTER_LEVEL LOG_LEVEL_DEBUG
#endif // NDEBUG
#endif // !LOG_FILTER_LEVEL

// Default log output
#ifndef LOG_OUTPUT_DEVICE
#ifdef _WIN32
#define LOG_OUTPUT_DEVICE LOG_DEVICE_DEBUG_PORT
#else
#define LOG_OUTPUT_DEVICE LOG_DEVICE_STDERR
#endif //_WIN32
#endif // !LOG_OUTPUT_DEVICE
#endif // NO_LOG

void _log_printf(const char *filename, const char *func, int line, const char *format, ...);

#if LOG_FILTER_LEVEL <= LOG_LEVEL_DEBUG
#define LOG_DEBUG(_fmt_, ...) _log_printf(__FILE__, __FUNCTION__, __LINE__, _fmt_, ##__VA_ARGS__)
#else
#define LOG_DEBUG(_fmt_, ...)
#endif

#if LOG_FILTER_LEVEL <= LOG_LEVEL_INFO
#define LOG_INFO(_fmt_, ...) _log_printf(__FILE__, __FUNCTION__, __LINE__, _fmt_, ##__VA_ARGS__)
#else
#define LOG_INFO(_fmt_, ...)
#endif

#if LOG_FILTER_LEVEL <= LOG_LEVEL_WARNING
#define LOG_WARNING(_fmt_, ...) _log_printf(__FILE__, __FUNCTION__, __LINE__, _fmt_, ##__VA_ARGS__)
#else
#define LOG_WARNING(_fmt_, ...)
#endif

#if LOG_FILTER_LEVEL <= LOG_LEVEL_ERROR
#define LOG_ERROR(_fmt_, ...) _log_printf(__FILE__, __FUNCTION__, __LINE__, _fmt_, ##__VA_ARGS__)
#else
#define LOG_ERROR(_fmt_, ...)
#endif

}; // namespace mimosa

#endif //_BASE_LOG_H_