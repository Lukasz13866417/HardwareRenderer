#ifndef LOG_HPP
#define LOG_HPP

#include <string>

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

class Logger {
public:
    static void init();

    static void info(const std::string &msg);
    static void debug(const std::string &msg);
    static void error(const std::string &msg);

private:
    static void logMessage(const std::string &level, const std::string &colorCode, const std::string &msg);
    static std::string currentTime();
    static std::string currentDateTime();
};

// Macros controlling whether logging calls actually do anything
#if LOG_ENABLE
    #define INFO(msg)  Logger::info(msg)
    #define DEBUG(msg) Logger::debug(msg)
    #define ERR(msg)   Logger::error(msg)
#else
    // If LOG_ENABLE=0, all logging calls are compiled out
    #define INFO(msg)  (void*)(nullptr)
    #define DEBUG(msg) (void*)(nullptr)
    #define ERR(msg)   (void*)(nullptr)
#endif

#endif // LOG_HPP
