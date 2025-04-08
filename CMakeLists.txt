cmake_minimum_required(VERSION 3.10)
project(HardwareRenderer LANGUAGES CXX)

find_package(OpenCL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(app
    main.cpp
    rendering_api/rendering_pipeline/gpu_context.cpp
)

target_include_directories(app PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/rendering_api/rendering_pipeline"
)

target_include_directories(app PRIVATE ${OpenCL_INCLUDE_DIRS})
target_link_libraries(app PRIVATE ${OpenCL_LIBRARIES})
target_include_directories(app PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(app PRIVATE ${SDL2_LIBRARIES})
target_include_directories(app PRIVATE ${SDL2_ttf_INCLUDE_DIRS})
target_link_libraries(app PRIVATE ${SDL2_ttf_LIBRARIES})
target_link_libraries(app PRIVATE SDL2_ttf::SDL2_ttf)