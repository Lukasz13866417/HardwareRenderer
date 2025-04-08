#ifndef GPU_CONTEXT_HPP
#define GPU_CONTEXT_HPP
#define CL_HPP_TARGET_OPENCL_VERSION 300
#include <CL/opencl.hpp>
#include <vector>

// Initializes the GPU context, device, platform, and queue.
// Returns true if initialization was successful, false otherwise.
bool initGPUContext();

// Returns the OpenCL command queue (C++ wrapper).
cl::CommandQueue getQueue();

// Returns the selected OpenCL device (C++ wrapper).
cl::Device getDevice();

// Returns the selected OpenCL platform (C++ wrapper).
cl::Platform getPlatform();

// Returns whether the GPU context has been successfully initialized.
bool isGPUContextInitialized();

void ASSERT_CL_OK(cl_int code);

#endif // GPU_CONTEXT_HPP
