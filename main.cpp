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
        //Float f = 1.0;
        //Double d = 2.0f;
        //Double xd = f + d;
        Float fl = 1.0f;
        HWR_FOR(Int32 a = 0; a < 10; ++a) {
            HWR_FOR(Int32 b = 0; b < 10; ++b) {
                HWR_IF(a+b==0){
                    Double xd = -1.0f + fl;
                }
            }
            Float fl2 = 1;
            fl -= a;
            ++a;
        }
    }};
    // This will call the lambda passed to the constructor.
    // When the lambda is called, the code inside it is run, 
    // and my custom types (Float etc.) generate the OpenCL code automatically,
    // thanks to operator overloads (+, *, -, / etc).
    std::string res = sum.compile();
    // print the compiled OpenCL code.
    std::cout<<res<<std::endl;
    
    
}