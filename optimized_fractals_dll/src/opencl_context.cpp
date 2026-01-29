#include "opencl_context.h"
#include <vector>
#include <stdexcept>
#include "opencl_check.h"

OpenCLContext init_opencl(const char** kernel_sources, size_t num_kernel_sources) {

    cl_int err = CL_SUCCESS; // Stores error messages

    OpenCLContext ctx{};

    cl_platform_id platform;
    CL_CHECK(clGetPlatformIDs(1, &platform, nullptr));

    CL_CHECK(clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &ctx.device, nullptr));

    ctx.context = clCreateContext(nullptr, 1, &ctx.device, nullptr, nullptr, &err);
    CL_CHECK(err);

    ctx.queue = clCreateCommandQueue(ctx.context, ctx.device, 0, &err);
    CL_CHECK(err);

    ctx.program = clCreateProgramWithSource(
        ctx.context, num_kernel_sources, kernel_sources, nullptr, &err
    );

    err = clBuildProgram(ctx.program, 1, &ctx.device, nullptr, nullptr, nullptr);

    if (err != CL_SUCCESS) {
        size_t log_size = 0;
        clGetProgramBuildInfo(
            ctx.program,
            ctx.device,
            CL_PROGRAM_BUILD_LOG,
            0, nullptr,
            &log_size
        );

        std::string log(log_size, '\0');
        clGetProgramBuildInfo(
            ctx.program,
            ctx.device,
            CL_PROGRAM_BUILD_LOG,
            log_size,
            log.data(),
            nullptr
        );

        throw std::runtime_error("OpenCL build error:\n" + log);
    }

    // Add a line here for each kernel
    ctx.standard_fractal_kernel = clCreateKernel(ctx.program, "standard_fractal", &err);
    CL_CHECK(err);
    
    ctx.slow_fractal_kernel = clCreateKernel(ctx.program, "slow_fractal", &err);
    CL_CHECK(err);

    ctx.boolean_fractal_kernel = clCreateKernel(ctx.program, "boolean_fractal", &err);
    CL_CHECK(err);

    ctx.standard_julia_kernel = clCreateKernel(ctx.program, "standard_julia", &err);
    CL_CHECK(err);

    return ctx;
}

void cleanup_opencl(OpenCLContext& ctx) {

    // Each kernel should get its own release
    if (ctx.standard_fractal_kernel)
        clReleaseKernel(ctx.standard_fractal_kernel);

    if (ctx.slow_fractal_kernel)
        clReleaseKernel(ctx.slow_fractal_kernel);

    if (ctx.boolean_fractal_kernel)
        clReleaseKernel(ctx.boolean_fractal_kernel);

    if (ctx.standard_julia_kernel)
        clReleaseKernel(ctx.standard_julia_kernel);

    if (ctx.program)
        clReleaseProgram(ctx.program);

    if (ctx.queue)
        clReleaseCommandQueue(ctx.queue);

    if (ctx.context)
        clReleaseContext(ctx.context);
}