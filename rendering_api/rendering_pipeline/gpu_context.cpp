#include "gpu_context.hpp"
#include "../log/log.hpp"
#include <CL/opencl.hpp>
#include <iostream>
#include <stdexcept>
#include <cassert>

// Keep these in an anonymous namespace so they're private to this .cpp.
namespace
{
    bool          g_initialized  = false;
    cl::Context   g_context;
    cl::CommandQueue g_commandQueue;
    cl::Device    g_device;
    cl::Platform  g_platform;
}

bool initGPUContext()
{
    if (g_initialized)
        return true;

    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    if (platforms.empty())
    {
        ERR("No OpenCL platforms found.");
        return false;
    }

    g_platform = platforms[0];

    std::vector<cl::Device> devices;
    g_platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
    if (devices.empty())
    {
        ERR("No GPU devices found, trying CPU...");
        g_platform.getDevices(CL_DEVICE_TYPE_CPU, &devices);
        if (devices.empty())
        {
            ERR("No CPU devices found either.");
            return false;
        }
    }

    g_device = devices[0];

    g_context = cl::Context({g_device});

    g_commandQueue = cl::CommandQueue(g_context, g_device);

    g_initialized = true;
    return true;
}

cl::CommandQueue getQueue()
{
    return g_commandQueue;
}

cl::Device getDevice()
{
    return g_device;
}

cl::Platform getPlatform()
{
    return g_platform;
}

bool isGPUContextInitialized()
{
    return g_initialized;
}

void ASSERT_CL_OK(cl_int code) {
    if (code == CL_SUCCESS)
        return;  // Everything is OK, do nothing.

    std::string errStr = "Unknown OpenCL error";
    switch(code) {
        // run-time and JIT compiler errors
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
    
    ERR("OpenCL error: " + errStr + " (" + std::to_string(code) + ")");
    assert(false); 
}
