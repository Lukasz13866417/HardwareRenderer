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


    //  shader. Is compiled to OpenCL code.
    hwr::Program sum{[](){
        Float a = 1.0f;
        Float b = 3.0f;
        Float c = a + b;
        Float d = c * (a - b * (a*a)) * 0.5f;
    }};
    // This will call the lambda passed to the constructor.
    // When the lambda is called, the code inside it is run, 
    // and my custom types (Float etc.) generate the OpenCL code automatically,
    // thanks to operator overloads (+, *, -, /).
    std::string res = sum.compile();
    // print the compiles OpenCL code.
    std::cout<<res<<std::endl;
    
    
}