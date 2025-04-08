cmake_minimum_required(VERSION 3.10)
project(HardwareRenderer LANGUAGES CXX)

find_package(OpenCL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Define a helper function that sets up common target properties
function(add_app_target target_name)
    add_executable(${target_name}
        main.cpp
        rendering_api/rendering_pipeline/gpu_context.cpp
        rendering_api/log/log.cpp
    )
    target_include_directories(${target_name} PRIVATE 
        "${CMAKE_CURRENT_SOURCE_DIR}/rendering_api/rendering_pipeline"
        "${CMAKE_CURRENT_SOURCE_DIR}/rendering_api/log"
    )
    target_include_directories(${target_name} PRIVATE ${OpenCL_INCLUDE_DIRS})
    target_link_libraries(${target_name} PRIVATE ${OpenCL_LIBRARIES})
    target_include_directories(${target_name} PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(${target_name} PRIVATE ${SDL2_LIBRARIES})
    target_include_directories(${target_name} PRIVATE ${SDL2_ttf_INCLUDE_DIRS})
    target_link_libraries(${target_name} PRIVATE ${SDL2_ttf_LIBRARIES})
    target_link_libraries(${target_name} PRIVATE SDL2_ttf::SDL2_ttf)
endfunction()

# Log both in file and console
add_app_target(app_default)

# Zero logging
add_app_target(app_nodebug)
target_compile_definitions(app_nodebug PRIVATE LOG_ENABLE=0)

# Log only to file
add_app_target(app_no_console)
target_compile_definitions(app_no_console PRIVATE LOG_ENABLE_CONSOLE=0)

# Log only to console
add_app_target(app_no_file)
target_compile_definitions(app_no_file PRIVATE LOG_ENABLE_FILE=0)
