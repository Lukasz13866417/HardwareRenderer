#ifndef HWR_GPU_CONTEXT_HPP
#define HWR_GPU_CONTEXT_HPP
#include "../gpu_cl_init.hpp"
#include <string>
#include <vector>
#include <optional>
#include <stdexcept>


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

}

#endif // HWR_GPU_CONTEXT_HPP
