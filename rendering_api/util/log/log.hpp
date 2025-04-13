#ifndef LOG_HPP
#define LOG_HPP
#include "log_config_detail.hpp"
#include "../../rendering_pipeline/gpu/gpu_cl_init.hpp"
#include "CL/opencl.hpp"
#include <string>
#include <stdexcept>

namespace hwr {
    namespace detail {
        void logInit();
        void Debug(const std::string &msg);
        void Error(const std::string &msg);
        void Info(const std::string &msg);     
        void Fatal(const std::string &msg);
        void Assert(bool cond, const std::string &msg);
        void Assert_CL_OK(cl_int code, const std::string &msg);
        void Success(const std::string &msg);
    };
} // namespace hwr


#if HWR_LOG_ENABLE
    #define HWR_LOG_INIT() hwr::detail::logInit()
    #define HWR_INFO(msg)  hwr::detail::Info(msg)
    #define HWR_DEBUG(msg) hwr::detail::Debug(msg)
    #define HWR_ERR(msg)   hwr::detail::Error(msg)
    #define HWR_FATAL(msg) hwr::detail::Fatal(msg)
    #define HWR_SUCCESS(msg) hwr::detail::Success(msg)
#else
    #define HWR_LOG_INIT() (void*)(nullptr)
    #define HWR_INFO(msg)  (void*)(nullptr)
    #define HWR_DEBUG(msg) (void*)(nullptr)
    #define HWR_ERR(msg)   (void*)(nullptr)
    #define HWR_FATAL(msg) throw std::runtime_error(msg)
    #define HWR_SUCCESS(msg) (void*)(nullptr)
#endif

#ifndef NDEBUG
    #define HWR_ASSERT(cond, msg) hwr::detail::Assert(cond, msg)
    #define HWR_ASSERT_CL_OK(code, when)     hwr::detail::Assert_CL_OK(code, when)
#else
    #define HWR_ASSERT(cond, msg)   (void*)(nullptr)
    #define HWR_ASSERT_CL_OK(code, msg) (void*)(nullptr)
#endif

#endif // LOG_HPP
