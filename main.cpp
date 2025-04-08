#include "rendering_api/rendering_pipeline/gpu_context.hpp"
#include<cassert>
#include<iostream>

int main(){
    if(!initGPUContext()){
        std::cerr<<"OpenCL init fail. Can't continue"<<std::endl;
    }else{
        std::cerr<<"OpenCL init OK."<<std::endl;
    }
}