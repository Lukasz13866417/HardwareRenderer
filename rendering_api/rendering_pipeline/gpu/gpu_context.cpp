#include "../util/log/log.hpp"
#include "gpu_context.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <stdexcept>
#include <cassert>


namespace hwr{

std::optional<GPUContext> initGPUContext()
{
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    if (platforms.empty())
    {
        HWR_ERR("No OpenCL platforms found.");
        return std::nullopt;
    }

    cl::Platform platform = platforms.front();

    std::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
    if (devices.empty())
    {
        HWR_ERR("No GPU devices found on the platform.");
        return std::nullopt;
    }

    cl::Device device = devices.front();

    cl_int err = CL_SUCCESS;
    cl::Context context({ device }, nullptr, nullptr, nullptr, &err);
    if(err != CL_SUCCESS)
    {
        HWR_ERR("Failed to create OpenCL context. Error code: " + std::to_string(err));
        return std::nullopt;
    }

    cl::CommandQueue queue(context, device, 0, &err);
    if(err != CL_SUCCESS)
    {
        HWR_ERR("Failed to create CommandQueue. Error code: " + std::to_string(err));
        return std::nullopt;
    }

    GPUContext contextObj(platform, device, context, queue);
    return contextObj;
}

std::optional<std::string> getFileContent(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        HWR_ERR("Failed to open kernel source file: " + filename);
        return std::nullopt;
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}


GPUContext::GPUContext(const cl::Platform&    platform,
                        const cl::Device&      device,
                        const cl::Context&     context,
                        const cl::CommandQueue& queue)
    : m_platform(platform)
    , m_device(device)
    , m_context(context)
    , m_queue(queue)
{} 

} // namespace hwr