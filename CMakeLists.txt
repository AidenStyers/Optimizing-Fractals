cmake_minimum_required(VERSION 3.15)

project(fractal_generation LANGUAGES CXX)

# Build a DLL
add_library(fractal_generation SHARED
    fractal_generation.cpp
    lodepng.cpp
)

# Use modern C++
target_compile_features(fractal_generation PRIVATE cxx_std_17)

# Ensure Windows export symbols
if (MSVC)
    target_compile_definitions(fractal_generation PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# Output DLL to project root for easy ctypes loading
set_target_properties(fractal_generation PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)