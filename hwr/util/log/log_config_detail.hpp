#ifndef LOG_CONFIG_DETAIL_HPP
#define LOG_CONFIG_DETAIL_HPP

#ifdef NDEBUG
    #define HWR_LOG_ENABLE 0
    #define NO_HWR_ASSERT 1
#endif

#ifndef HWR_LOG_ENABLE
    #define HWR_LOG_ENABLE 1
#else
    #if HWR_LOG_ENABLE == 0
        #define LOG_ENABLE_CONSOLE 0
        #define LOG_ENABLE_FILE 0
    #endif
#endif

#ifndef LOG_ENABLE_CONSOLE
    #define LOG_ENABLE_CONSOLE 1
#endif

#ifndef LOG_ENABLE_FILE
    #define LOG_ENABLE_FILE 1
#endif

#ifndef LOG_FILE_PATH
    #define LOG_FILE_PATH "./logs.log"
#endif

#define ANSI_COLOR_RESET        "\x1b[0m"
#define ANSI_COLOR_RED          "\x1b[31m"
#define ANSI_COLOR_DARK_RED     "\x1b[38;5;52m"
#define ANSI_COLOR_DARK_GREEN   "\x1b[38;5;22m"
#define ANSI_COLOR_GREEN        "\x1b[32m"
#define ANSI_COLOR_YELLOW       "\x1b[33m"
#define ANSI_COLOR_LIGHT_RED    "\033[1;31m"

#endif // LOG_CONFIG_DETAIL_HPP
