#ifndef LOG_HPP
#define LOG_HPP

#include "log_config.hpp"
#include <string>

namespace hwr {
        void logInit();
        void debug(const std::string &msg);
        void error(const std::string &msg);
        void info(const std::string &msg);     
} // namespace hwr

#if LOG_ENABLE
    #define HWR_LOG_INIT() hwr::logInit()
    #define HWR_INFO(msg)  hwr::info(msg)
    #define HWR_DEBUG(msg) hwr::debug(msg)
    #define HWR_ERR(msg)   hwr::error(msg)
#else
    #define HWR_LOG_INIT() (void*)(nullptr)
    #define HWR_INFO(msg)  (void*)(nullptr)
    #define HWR_DEBUG(msg) (void*)(nullptr)
    #define HWR_ERR(msg)   (void*)(nullptr)
#endif

#endif // LOG_HPP
