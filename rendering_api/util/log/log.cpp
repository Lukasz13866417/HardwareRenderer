#include "log.hpp"
#if LOG_ENABLE
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace hwr {

        namespace{
            static std::ofstream g_logFile;
            std::string currentTime() {
                using std::chrono::system_clock;
                auto now = system_clock::now();
                auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                            now.time_since_epoch()) % 1000;
                std::time_t t = system_clock::to_time_t(now);
        
                std::tm tmBuf;
            #ifdef _WIN32
                localtime_s(&tmBuf, &t);
            #else
                localtime_r(&t, &tmBuf);
            #endif
        
                std::ostringstream oss;
                oss << std::put_time(&tmBuf, "%T") << "." << std::setw(3)
                    << std::setfill('0') << ms.count();
                return oss.str();
            }
        
            std::string currentDateTime() {
                std::time_t now = std::time(nullptr);
                std::tm tmBuf;
            #ifdef _WIN32
                localtime_s(&tmBuf, &now);
            #else
                localtime_r(&now, &tmBuf);
            #endif
                char buffer[64];
                strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tmBuf);
                return std::string(buffer);
            }
            void logMessage(const std::string &level,
                const std::string &colorCode,
                const std::string &msg)
            {
            std::string timeStr = currentTime();
            std::string line = "[" + timeStr + "] [" + level + "] " + msg;

            #if LOG_ENABLE_CONSOLE
            std::cout << colorCode << line << ANSI_COLOR_RESET << std::endl;
            #endif

            #if LOG_ENABLE_FILE
            if (g_logFile.is_open()) {
            g_logFile << line << std::endl;
            }
            #endif
            }
        }

    void logInit() {
    #if LOG_ENABLE_FILE
        // Only open a file if file logging is on
        g_logFile.open(LOG_FILE_PATH, std::ios::app);
        if (!g_logFile.is_open()) {
            std::cerr << "init() - Could not open log file: "
                      << LOG_FILE_PATH << std::endl;
        }
    #endif
        // Print a separator in both file and console so we can see new runs.
        std::string initMsg = "---------------------------------------- "
                              "Log started at: " + currentDateTime() + " " + "---------------------------------------- ";
        logMessage("INIT", ANSI_COLOR_GREEN, initMsg);
    }

    void info(const std::string &msg) {
        logMessage("INFO", ANSI_COLOR_GREEN, msg);
    }

    void debug(const std::string &msg) {
        logMessage("DEBUG", ANSI_COLOR_YELLOW, msg);
    }

    void error(const std::string &msg) {
        logMessage("ERROR", ANSI_COLOR_RED, msg);
    }

} // namespace hwr
#endif