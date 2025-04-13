#include "rendering_api/rendering_pipeline/gpu/gpu_context.hpp"
#include "rendering_api/util/log/log.hpp"
#include "rendering_api/rendering_pipeline/gpu/gpu_buffer.hpp"
#include "rendering_api/util/math/math_util.hpp"
#include<cassert>
#include<iostream>

using vec4 = hwr::vec4f;

int main(){

    HWR_LOG_INIT();

    std::optional<hwr::GPUContext> _gpuContext = hwr::initGPUContext();
    if(!_gpuContext){
        HWR_FATAL("OpenCL init fail. Can't continue");
    }
    HWR_INFO("OpenCL init OK.");
    hwr::GPUContext gpu_context = _gpuContext.value();
    HWR_DEBUG("SOMETHING!");

    //HWR_ASSERT(false,"WTF");
    //HWR_ASSERT_CL_OK(1,"HUH");

    HWR_SUCCESS("Everything works because nothing's been tested.");

    hwr::AllPurposeBuffer apb(gpu_context, 2, std::vector<vec4>{vec4(1,2,3,4),vec4(5,6,7,8)});
    HWR_SUCCESS("All purpose buffer has been created.");
    vec4 vecs[2];
    apb.readTo(std::span<vec4>(vecs,2));
    HWR_SUCCESS("Some vector data has been read.");
    std::cout<<hwr::to_string(vecs[0])<<std::endl;
    std::cout<<hwr::to_string(vecs[1])<<std::endl;
    HWR_INFO("The end.");

}