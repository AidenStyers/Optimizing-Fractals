#include "optimized_fractals.h"
#include "opencl_context.h"
#include "kernel_source.h"
#include "opencl_check.h"
#include <fstream>
#include <vector>
#include <string>


// Stores last error
static thread_local std::string last_error;

// Function to get error message from C in python
const char* get_last_error() {
    return last_error.c_str();
}


int standard_fractal(
    float cx, // X position of center 
    float cy, // Y position of center
    float scale, // Scale of fractal, how wide/long each pixel of the resulting image is in the complex plane
    int width, // Width of resulting image 
    int height, // Height of resulting image
    int max_iter, // Maximum number of iterations
    int* color_palette, // 4x3 array of colors to use for palette, the four rows each are the RGB for a color used
    unsigned char* output,
    float* coeffs
) {
    try {
        static OpenCLContext ctx;
        static bool initialized = false;

        if (!initialized) {
            ctx = init_opencl(all_kernel_sources, num_kernel_sources);
            initialized = true;
        }

        // Create buffer to handle output
        size_t buffer_size = width * height * 3;
        cl_mem out_buf = clCreateBuffer(
            ctx.context, CL_MEM_WRITE_ONLY,
            buffer_size, nullptr, nullptr
        );

        // Create buffer to handle coeffs
        cl_mem coeffs_buf = clCreateBuffer(
            ctx.context,
            CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(float) * 52, // Amount of parameters
            coeffs,
            nullptr
        );

        // Create buffer to handle color_coeffs
        cl_mem color_palette_buf = clCreateBuffer(
            ctx.context,
            CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(int) * 12, // Amount of parameters
            color_palette,
            nullptr
        );

        cl_kernel kernel = ctx.standard_fractal_kernel;

        CL_CHECK(clSetKernelArg(kernel, 0, sizeof(float), &cx));
        CL_CHECK(clSetKernelArg(kernel, 1, sizeof(float), &cy));
        CL_CHECK(clSetKernelArg(kernel, 2, sizeof(float), &scale));
        CL_CHECK(clSetKernelArg(kernel, 3, sizeof(int), &width));
        CL_CHECK(clSetKernelArg(kernel, 4, sizeof(int), &height));
        CL_CHECK(clSetKernelArg(kernel, 5, sizeof(int), &max_iter));
        CL_CHECK(clSetKernelArg(kernel, 6, sizeof(cl_mem), &color_palette_buf));
        CL_CHECK(clSetKernelArg(kernel, 7, sizeof(cl_mem), &out_buf));
        CL_CHECK(clSetKernelArg(kernel, 8, sizeof(cl_mem), &coeffs_buf));

        size_t global[2] = { (size_t)width, (size_t)height };
        CL_CHECK(clEnqueueNDRangeKernel(ctx.queue, kernel, 2, nullptr, global, nullptr, 0, nullptr, nullptr));

        CL_CHECK(clEnqueueReadBuffer(
            ctx.queue, out_buf, CL_TRUE, 0,
            buffer_size, output, 0, nullptr, nullptr
        ));

        CL_CHECK(clReleaseMemObject(coeffs_buf));
        CL_CHECK(clReleaseMemObject(color_palette_buf));
        CL_CHECK(clReleaseMemObject(out_buf));

        return 0;
    }
    catch (const std::exception& e) {
        last_error = e.what();
        return -1;
    }
}

int slow_fractal(
    float cx, // X position of center 
    float cy, // Y position of center
    float scale, // Scale of fractal, how wide/long each pixel of the resulting image is in the complex plane
    int width, // Width of resulting image 
    int height, // Height of resulting image
    int max_iter, // Maximum number of iterations
    unsigned char* output,
    float* coeffs
) {
    try {
        static OpenCLContext ctx;
        static bool initialized = false;

        if (!initialized) {
            ctx = init_opencl(all_kernel_sources, num_kernel_sources);
            initialized = true;
        }

        // Create buffer to handle output
        size_t buffer_size = width * height * 3;
        cl_mem out_buf = clCreateBuffer(
            ctx.context, CL_MEM_WRITE_ONLY,
            buffer_size, nullptr, nullptr
        );

        // Create buffer to handle coeffs
        cl_mem coeffs_buf = clCreateBuffer(
            ctx.context,
            CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(float) * 52, // Amount of parameters
            coeffs,
            nullptr
        );

        cl_kernel kernel = ctx.slow_fractal_kernel;

        CL_CHECK(clSetKernelArg(kernel, 0, sizeof(float), &cx));
        CL_CHECK(clSetKernelArg(kernel, 1, sizeof(float), &cy));
        CL_CHECK(clSetKernelArg(kernel, 2, sizeof(float), &scale));
        CL_CHECK(clSetKernelArg(kernel, 3, sizeof(int), &width));
        CL_CHECK(clSetKernelArg(kernel, 4, sizeof(int), &height));
        CL_CHECK(clSetKernelArg(kernel, 5, sizeof(int), &max_iter));
        CL_CHECK(clSetKernelArg(kernel, 6, sizeof(cl_mem), &out_buf));
        CL_CHECK(clSetKernelArg(kernel, 7, sizeof(cl_mem), &coeffs_buf));

        size_t global[2] = { (size_t)width, (size_t)height };
        CL_CHECK(clEnqueueNDRangeKernel(ctx.queue, kernel, 2, nullptr, global, nullptr, 0, nullptr, nullptr));

        CL_CHECK(clEnqueueReadBuffer(
            ctx.queue, out_buf, CL_TRUE, 0,
            buffer_size, output, 0, nullptr, nullptr
        ));

        CL_CHECK(clReleaseMemObject(coeffs_buf));
        CL_CHECK(clReleaseMemObject(out_buf));

        return 0;
    }
    catch (const std::exception& e) {
        last_error = e.what();
        return -1;
    }
}



int boolean_fractal(
    float cx, // X position of center 
    float cy, // Y position of center
    float scale, // Scale of fractal, how wide/long each pixel of the resulting image is in the complex plane
    int width, // Width of resulting image 
    int height, // Height of resulting image
    int max_iter, // Maximum number of iterations
    unsigned char* output,
    float* coeffs
) {
    try {
        static OpenCLContext ctx;
        static bool initialized = false;

        if (!initialized) {
            ctx = init_opencl(all_kernel_sources, num_kernel_sources);
            initialized = true;
        }

        // Create buffer to handle output
        size_t buffer_size = width * height;
        cl_mem out_buf = clCreateBuffer(
            ctx.context, CL_MEM_WRITE_ONLY,
            buffer_size, nullptr, nullptr
        );

        // Create buffer to handle coeffs
        cl_mem coeffs_buf = clCreateBuffer(
            ctx.context,
            CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(float) * 52, // Amount of parameters
            coeffs,
            nullptr
        );

        cl_kernel kernel = ctx.standard_fractal_kernel;

        CL_CHECK(clSetKernelArg(kernel, 0, sizeof(float), &cx));
        CL_CHECK(clSetKernelArg(kernel, 1, sizeof(float), &cy));
        CL_CHECK(clSetKernelArg(kernel, 2, sizeof(float), &scale));
        CL_CHECK(clSetKernelArg(kernel, 3, sizeof(int), &width));
        CL_CHECK(clSetKernelArg(kernel, 4, sizeof(int), &height));
        CL_CHECK(clSetKernelArg(kernel, 5, sizeof(int), &max_iter));
        CL_CHECK(clSetKernelArg(kernel, 6, sizeof(cl_mem), &out_buf));
        CL_CHECK(clSetKernelArg(kernel, 7, sizeof(cl_mem), &coeffs_buf));

        size_t global[2] = { (size_t)width, (size_t)height };
        CL_CHECK(clEnqueueNDRangeKernel(ctx.queue, kernel, 2, nullptr, global, nullptr, 0, nullptr, nullptr));

        CL_CHECK(clEnqueueReadBuffer(
            ctx.queue, out_buf, CL_TRUE, 0,
            buffer_size, output, 0, nullptr, nullptr
        ));

        CL_CHECK(clReleaseMemObject(coeffs_buf));
        CL_CHECK(clReleaseMemObject(out_buf));

        return 0;
    }
    catch (const std::exception& e) {
        last_error = e.what();
        return -1;
    }
}