#pragma once
#define CL_TARGET_OPENCL_VERSION 120
#include <CL/cl.h>

struct OpenCLContext {
    cl_context context;
    cl_command_queue queue;
    cl_program program;

    // Each kernel should get its own line here
    cl_kernel standard_fractal_kernel;
    cl_kernel slow_fractal_kernel;

    cl_device_id device;
};

OpenCLContext init_opencl(const char** kernel_sources, size_t num_kernel_sources);
void cleanup_opencl(OpenCLContext& ctx);