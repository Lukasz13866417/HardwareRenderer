#include "rendering_api/rendering_pipeline/gpu/gpu_context.hpp"
#include "rendering_api/util/log/log.hpp"
#include "rendering_api/rendering_pipeline/gpu/gpu_buffer.hpp"
#include "rendering_api/util/math/math_util"
#include<cassert>
#include<iostream>

int main(){

    HWR_LOG_INIT();

    std::optional<hwr::GPUContext> _gpuContext = hwr::initGPUContext();
    if(!_gpuContext){
        HWR_FATAL("OpenCL init fail. Can't continue");
    }
    HWR_INFO("OpenCL init OK.");
    hwr::GPUContext gpu_context = _gpuContext.value();
    HWR_DEBUG("SOMETHING!");

    hwr::WriteOnceBuffer<hwr::vec4f> buf(gpu_context,2,std::vector<hwr::vec4f>({{1,1,1,1},{0,0,0,0}}));

    
}