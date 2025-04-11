#include "rendering_api/rendering_pipeline/gpu_context.hpp"
#include "rendering_api/util/log/log.hpp"
#include<cassert>
#include<iostream>

int main(){

    HWR_LOG_INIT();

    std::optional<hwr::GPUContext> _gpuContext = hwr::initGPUContext();
    if(!_gpuContext){
        HWR_ERR("OpenCL init fail. Can't continue");
        return 1;
    }
    HWR_INFO("OpenCL init OK.");
    hwr::GPUContext gpu_context = _gpuContext.value();
    HWR_DEBUG("SOMETHING!");
}