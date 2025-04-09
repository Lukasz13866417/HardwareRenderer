#include "rendering_api/rendering_pipeline/gpu_context.hpp"
#include "rendering_api/log/log.hpp"
#include<cassert>
#include<iostream>

int main(){

    Logger::init();

    std::optional<GPUContext> _gpuContext = initGPUContext();
    if(!_gpuContext){
        ERR("OpenCL init fail. Can't continue");
        return 1;
    }
    INFO("OpenCL init OK.");
    GPUContext gpu_context = _gpuContext.value();
    DEBUG("SOMETHING!");
}