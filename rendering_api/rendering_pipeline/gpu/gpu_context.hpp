#ifndef GPU_CONTEXT_HPP
#define GPU_CONTEXT_HPP
#include "./gpu_assert_macros_detail.hpp"
#include <CL/opencl.hpp>
#include <string>
#include <vector>
#include <optional>
#include <stdexcept>



namespace hwr {

    // Forward declaration for friend class usage
    class GPUContext;

    // Public API
    /**
    * \brief Create and initialize a GPUContext.
    * \return A GPUContext if successful; otherwise, std::nullopt.
    */
    std::optional<GPUContext> initGPUContext();

    /**
    * \brief Utility to read file content into a string.
    */
    std::string getFileContent(const std::string filename);

    /**
    * \brief Exits if the OpenCL error code indicates a failure. Dont use this function directly - use HWR_ASSERT_CL_OK
    */
    void __assert_cl_ok(cl_int code, const std::string& situation="not specified");

    namespace detail {
        class GPUAccess;
    }

    /**
    * \class GPUContext
    * \brief Encapsulates the OpenCL context, device, queue, etc.
    */
    class GPUContext {
    private:
        // Only friend can construct
        GPUContext(const cl::Platform&     platform,
                   const cl::Device&       device,
                   const cl::Context&      context,
                   const cl::CommandQueue& queue);

        cl::Platform     m_platform;
        cl::Device       m_device;
        cl::Context      m_context;
        cl::CommandQueue m_queue;

        friend std::optional<GPUContext> initGPUContext();
        friend class detail::GPUAccess;
    };

    namespace detail {
        /**
        * \class GPUAccess
        * \brief Provides internal access to GPUContext's members.
        */
        class GPUAccess {
            friend class GPUContext;

        public:
            static cl::Platform&     getPlatform(GPUContext& ctx)   { return ctx.m_platform; }
            static cl::Device&       getDevice(GPUContext& ctx)     { return ctx.m_device;   }
            static cl::Context&      getContext(GPUContext& ctx)    { return ctx.m_context;  }
            static cl::CommandQueue& getQueue(GPUContext& ctx)      { return ctx.m_queue;    }
        };
    }

} // namespace hwr

#endif // GPU_CONTEXT_HPP
