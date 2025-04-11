#ifndef LOG_CONFIG_HPP
#define LOG_CONFIG_HPP

#ifndef LOG_ENABLE
    #define LOG_ENABLE 1
#else
    #if LOG_ENABLE == 0
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

#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"

#endif // LOG_CONFIG_HPP
