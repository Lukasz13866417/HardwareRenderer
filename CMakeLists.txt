cmake_minimum_required(VERSION 3.10)
project(HardwareRenderer LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

find_package(OpenCL REQUIRED)
# SDL just for the demo.
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # Disable compiler-specific extensions like GNU extensions


set(COMMON_WARNINGS "-Wall" "-Wextra" "-Wpedantic" "-Werror" "-O3")

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    list(APPEND COMMON_WARNINGS "-Wconversion" "-Wsign-conversion" "-Wshadow")
endif()

# Define a helper function that sets up common target properties
function(add_app_target target_name)
    add_executable(${target_name}
        main.cpp
        hwr/rendering_pipeline/gpu/context/gpu_context.cpp
        hwr/util/log/log.cpp
        hwr/util/math/math_util.cpp

    )
    target_include_directories(${target_name} PRIVATE 
        "${CMAKE_CURRENT_SOURCE_DIR}/hwr/include"
    )
    target_include_directories(${target_name} PRIVATE ${OpenCL_INCLUDE_DIRS})
    target_link_libraries(${target_name} PRIVATE ${OpenCL_LIBRARIES})
    target_include_directories(${target_name} PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(${target_name} PRIVATE ${SDL2_LIBRARIES})
    target_include_directories(${target_name} PRIVATE ${SDL2_ttf_INCLUDE_DIRS})
    target_link_libraries(${target_name} PRIVATE ${SDL2_ttf_LIBRARIES})
    target_link_libraries(${target_name} PRIVATE SDL2_ttf::SDL2_ttf)
    target_compile_options(${target_name} PRIVATE ${COMMON_WARNINGS})
endfunction()

# Log both in file and console
add_app_target(app_default)
target_compile_definitions(app_default PRIVATE LOG_ENABLE=1 LOG_ENABLE_CONSOLE=1 LOG_ENABLE_FILE=1)

# Zero logging & no asserts
add_app_target(app_no_debug)
target_compile_definitions(app_no_debug PRIVATE NDEBUG)

# Log only to file
add_app_target(app_no_console)
target_compile_definitions(app_no_console PRIVATE LOG_ENABLE=1 LOG_ENABLE_CONSOLE=0 LOG_ENABLE_FILE=1)

# Log only to console
add_app_target(app_no_file)
target_compile_definitions(app_no_file PRIVATE LOG_ENABLE=1 LOG_ENABLE_CONSOLE=1 LOG_ENABLE_FILE=0)
