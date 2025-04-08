#include "rendering_api/rendering_pipeline/gpu_context.hpp"
#include "rendering_api/log/log.hpp"
#include<cassert>
#include<iostream>

int main(){

    Logger::init();

    if(!initGPUContext()){
        ERR("OpenCL init fail. Can't continue");
    }else{
        INFO("OpenCL init OK.");
    }
}