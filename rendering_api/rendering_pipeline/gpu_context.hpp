#ifndef GPU_CONTEXT_HPP
#define GPU_CONTEXT_HPP
#define CL_HPP_TARGET_OPENCL_VERSION 300
#include <CL/opencl.hpp>
#include <string>
#include <vector>
#include <optional>
#include <stdexcept>
#ifndef NO_CL_ASSERT
#define NO_CL_ASSERT 0
#endif

namespace hwr{

    // Forward declaration of GPUContext
    class GPUContext;

    /**
    * \brief Create and initialize a GPUContext.
    *
    * \param kernelSource The kernel source string you want to build into the program.
    * \param errorMsg If an error occurs, an error message is written to this string.
    * \return A GPUContext if successful; otherwise, std::nullopt.
    */
    std::optional<hwr::GPUContext> initGPUContext();

    /**
    * \class GPUContext
    * \brief Encapsulates the OpenCL context, device, queue, program, etc.
    */
    class GPUContext {
    public:
        /// Accessors
        cl::Platform    getPlatform() const  { return m_platform; }
        cl::Device      getDevice()   const  { return m_device;   }
        cl::Context     getContext()  const  { return m_context;  }
        cl::CommandQueue getQueue()   const  { return m_queue;    }

    private:
        // Make constructor private so only friend (initGPUContext) can call it.
        GPUContext(const cl::Platform&    platform,
                const cl::Device&      device,
                const cl::Context&     context,
                const cl::CommandQueue& queue);

        // The OpenCL objects that this GPUContext manages
        cl::Platform    m_platform;
        cl::Device      m_device;
        cl::Context     m_context;
        cl::CommandQueue m_queue;
        cl::Program     m_program;

        // Allow our free function to construct GPUContext
        friend std::optional<GPUContext> initGPUContext();
    };

    std::string getFileContent(const std::string filename);

    void __assert_cl_ok(cl_int code);

}


#if NO_CL_ASSERT == 0
#define HWR_ASSERT_CL_OK(code) hwr::__assert_cl_ok(code)
#else
#define HWR_ASSERT_CL_OK(code) (void*)(nullptr)
#endif

#endif // GPU_CONTEXT_HPP
