#include <hwr/gpu_context.hpp>
#include <hwr/log.hpp>
#include <hwr/buffer.hpp>
#include <hwr/math.hpp>
#include <hwr/shader/types.hpp>
#include <hwr/shader/shader.hpp>
#include<cassert>
#include<iostream>
#include<string>

using vec4 = hwr::vec4f;

int main(){

    std::optional<hwr::GPUContext> _gpuContext = hwr::initGPUContext();
    if(!_gpuContext){
        HWR_FATAL("OpenCL init fail. Can't continue");
    }
    HWR_INFO("OpenCL init OK.");
    hwr::GPUContext gpu_context = _gpuContext.value();
    HWR_DEBUG("SOMETHING!");

    hwr::Program sum{[](){
        Float a = 1.0f;
        Float b = 3.0f;
        Float c = a + b;
        Float d = c * (a - b);
    }};
    std::string res = sum.compile();
    std::cout<<res<<std::endl;
    
    
}