cmake_minimum_required(VERSION 3.15)
project(optimized_fractals_dll)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCL REQUIRED)

add_library(optimized_fractals SHARED
    src/optimized_fractals.cpp
    src/opencl_context.cpp
    src/opencl_errors.cpp 
)

target_include_directories(optimized_fractals PUBLIC include)
target_link_libraries(optimized_fractals OpenCL::OpenCL)