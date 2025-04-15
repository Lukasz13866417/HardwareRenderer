#include <hwr/gpu_context.hpp>
#include <hwr/log.hpp>
#include <hwr/buffer.hpp>
#include <hwr/math.hpp>
#include <hwr/program.hpp>
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
    HWR_SUCCESS(hwr::to_string(vecs[0]));
    HWR_SUCCESS(hwr::to_string(vecs[1]));
    HWR_INFO("The end.");

    hwr::ConstBuffer cb(gpu_context, 2, std::vector<vec4>{vec4(1,2,3,4),vec4(5,6,7,8)});
    //cb.readTo(std::span<vec4>(vecs,2)); // shouldn't compile
    //cb.writeFrom(std::vector<vec4>{vec4(1,2,3,4),vec4(5,6,7,8)}); // shouldn't compile

    hwr::Program<float, vec4, vec4, vec4> prog;
    prog.generateCode();
    HWR_SUCCESS(prog.generateCode());
}