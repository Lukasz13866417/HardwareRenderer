#include "../util/log/log.hpp"
#include "gpu_context.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <CL/opencl.hpp>
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

void __assert_cl_ok(cl_int code, const std::string& situation) {
    if (code == CL_SUCCESS)
        return;  // Everything is OK, do nothing.

    std::string errStr = "Unknown OpenCL error";
    switch(code) {
        // run-time / JIT errors
        case CL_DEVICE_NOT_FOUND:                     errStr = "CL_DEVICE_NOT_FOUND"; break;
        case CL_DEVICE_NOT_AVAILABLE:                 errStr = "CL_DEVICE_NOT_AVAILABLE"; break;
        case CL_COMPILER_NOT_AVAILABLE:               errStr = "CL_COMPILER_NOT_AVAILABLE"; break;
        case CL_MEM_OBJECT_ALLOCATION_FAILURE:        errStr = "CL_MEM_OBJECT_ALLOCATION_FAILURE"; break;
        case CL_OUT_OF_RESOURCES:                     errStr = "CL_OUT_OF_RESOURCES"; break;
        case CL_OUT_OF_HOST_MEMORY:                   errStr = "CL_OUT_OF_HOST_MEMORY"; break;
        case CL_PROFILING_INFO_NOT_AVAILABLE:         errStr = "CL_PROFILING_INFO_NOT_AVAILABLE"; break;
        case CL_MEM_COPY_OVERLAP:                     errStr = "CL_MEM_COPY_OVERLAP"; break;
        case CL_IMAGE_FORMAT_MISMATCH:                errStr = "CL_IMAGE_FORMAT_MISMATCH"; break;
        case CL_IMAGE_FORMAT_NOT_SUPPORTED:           errStr = "CL_IMAGE_FORMAT_NOT_SUPPORTED"; break;
        case CL_BUILD_PROGRAM_FAILURE:                errStr = "CL_BUILD_PROGRAM_FAILURE"; break;
        case CL_MAP_FAILURE:                          errStr = "CL_MAP_FAILURE"; break;
        case CL_MISALIGNED_SUB_BUFFER_OFFSET:         errStr = "CL_MISALIGNED_SUB_BUFFER_OFFSET"; break;
        case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST: errStr = "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST"; break;
        case CL_COMPILE_PROGRAM_FAILURE:              errStr = "CL_COMPILE_PROGRAM_FAILURE"; break;
        case CL_LINKER_NOT_AVAILABLE:                 errStr = "CL_LINKER_NOT_AVAILABLE"; break;
        case CL_LINK_PROGRAM_FAILURE:                 errStr = "CL_LINK_PROGRAM_FAILURE"; break;
        case CL_DEVICE_PARTITION_FAILED:              errStr = "CL_DEVICE_PARTITION_FAILED"; break;
        case CL_KERNEL_ARG_INFO_NOT_AVAILABLE:        errStr = "CL_KERNEL_ARG_INFO_NOT_AVAILABLE"; break;

        // compile-time errors
        case CL_INVALID_VALUE:                        errStr = "CL_INVALID_VALUE"; break;
        case CL_INVALID_DEVICE_TYPE:                  errStr = "CL_INVALID_DEVICE_TYPE"; break;
        case CL_INVALID_PLATFORM:                     errStr = "CL_INVALID_PLATFORM"; break;
        case CL_INVALID_DEVICE:                       errStr = "CL_INVALID_DEVICE"; break;
        case CL_INVALID_CONTEXT:                      errStr = "CL_INVALID_CONTEXT"; break;
        case CL_INVALID_QUEUE_PROPERTIES:             errStr = "CL_INVALID_QUEUE_PROPERTIES"; break;
        case CL_INVALID_COMMAND_QUEUE:                errStr = "CL_INVALID_COMMAND_QUEUE"; break;
        case CL_INVALID_HOST_PTR:                     errStr = "CL_INVALID_HOST_PTR"; break;
        case CL_INVALID_MEM_OBJECT:                   errStr = "CL_INVALID_MEM_OBJECT"; break;
        case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:      errStr = "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR"; break;
        case CL_INVALID_IMAGE_SIZE:                   errStr = "CL_INVALID_IMAGE_SIZE"; break;
        case CL_INVALID_SAMPLER:                      errStr = "CL_INVALID_SAMPLER"; break;
        case CL_INVALID_BINARY:                       errStr = "CL_INVALID_BINARY"; break;
        case CL_INVALID_BUILD_OPTIONS:                errStr = "CL_INVALID_BUILD_OPTIONS"; break;
        case CL_INVALID_PROGRAM:                      errStr = "CL_INVALID_PROGRAM"; break;
        case CL_INVALID_PROGRAM_EXECUTABLE:           errStr = "CL_INVALID_PROGRAM_EXECUTABLE"; break;
        case CL_INVALID_KERNEL_NAME:                  errStr = "CL_INVALID_KERNEL_NAME"; break;
        case CL_INVALID_KERNEL_DEFINITION:            errStr = "CL_INVALID_KERNEL_DEFINITION"; break;
        case CL_INVALID_KERNEL:                       errStr = "CL_INVALID_KERNEL"; break;
        case CL_INVALID_ARG_INDEX:                    errStr = "CL_INVALID_ARG_INDEX"; break;
        case CL_INVALID_ARG_VALUE:                    errStr = "CL_INVALID_ARG_VALUE"; break;
        case CL_INVALID_ARG_SIZE:                     errStr = "CL_INVALID_ARG_SIZE"; break;
        case CL_INVALID_KERNEL_ARGS:                  errStr = "CL_INVALID_KERNEL_ARGS"; break;
        case CL_INVALID_WORK_DIMENSION:               errStr = "CL_INVALID_WORK_DIMENSION"; break;
        case CL_INVALID_WORK_GROUP_SIZE:              errStr = "CL_INVALID_WORK_GROUP_SIZE"; break;
        case CL_INVALID_WORK_ITEM_SIZE:               errStr = "CL_INVALID_WORK_ITEM_SIZE"; break;
        case CL_INVALID_GLOBAL_OFFSET:                errStr = "CL_INVALID_GLOBAL_OFFSET"; break;
        case CL_INVALID_EVENT_WAIT_LIST:              errStr = "CL_INVALID_EVENT_WAIT_LIST"; break;
        case CL_INVALID_EVENT:                        errStr = "CL_INVALID_EVENT"; break;
        case CL_INVALID_OPERATION:                    errStr = "CL_INVALID_OPERATION"; break;
        case CL_INVALID_GL_OBJECT:                    errStr = "CL_INVALID_GL_OBJECT"; break;
        default:                                      errStr = "Unknown OpenCL error"; break;
    }
    
    HWR_ERR("OpenCL error: " + errStr + " (" + std::to_string(code) + ")\nSituation: "+situation);
    assert(false); 
}

}