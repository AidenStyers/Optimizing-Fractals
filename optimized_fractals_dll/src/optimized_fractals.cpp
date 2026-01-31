#include "optimized_fractals.h"
#include "opencl_context.h"
#include "kernel_source.h"
#include "opencl_check.h"
#include <fstream>
#include <iostream>
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
    int bailout_radius,
    coloring_palette* coloring, 
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
        cl_mem coloring_buf = clCreateBuffer(
            ctx.context,
            CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            sizeof(coloring_palette), // Amount of parameters
            coloring,
            nullptr
        );

        cl_kernel kernel = ctx.standard_fractal_kernel;

        CL_CHECK(clSetKernelArg(kernel, 0, sizeof(float), &cx));
        CL_CHECK(clSetKernelArg(kernel, 1, sizeof(float), &cy));
        CL_CHECK(clSetKernelArg(kernel, 2, sizeof(float), &scale));
        CL_CHECK(clSetKernelArg(kernel, 3, sizeof(int), &width));
        CL_CHECK(clSetKernelArg(kernel, 4, sizeof(int), &height));
        CL_CHECK(clSetKernelArg(kernel, 5, sizeof(int), &max_iter));
        CL_CHECK(clSetKernelArg(kernel, 6, sizeof(int), &bailout_radius));
        CL_CHECK(clSetKernelArg(kernel, 7, sizeof(cl_mem), &coloring_buf));
        CL_CHECK(clSetKernelArg(kernel, 8, sizeof(cl_mem), &out_buf));
        CL_CHECK(clSetKernelArg(kernel, 9, sizeof(cl_mem), &coeffs_buf));

        size_t global[2] = { (size_t)width, (size_t)height };
        CL_CHECK(clEnqueueNDRangeKernel(ctx.queue, kernel, 2, nullptr, global, nullptr, 0, nullptr, nullptr));

        CL_CHECK(clEnqueueReadBuffer(
            ctx.queue, out_buf, CL_TRUE, 0,
            buffer_size, output, 0, nullptr, nullptr
        ));

        CL_CHECK(clReleaseMemObject(coeffs_buf));
        CL_CHECK(clReleaseMemObject(coloring_buf));
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

        cl_kernel kernel = ctx.boolean_fractal_kernel;

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



int coolness_raw(
    float cx, // X position of center 
    float cy, // Y position of center
    float scale, // Scale of fractal, how wide/long each pixel of the resulting image is in the complex plane
    int width, // Width of resulting image 
    int height, // Height of resulting image
    int max_iter, // Maximum number of iterations
    float* coeffs, // Coefficients to calculate fractal
    int* surface_area_out,
    int* size_out 
) {

    unsigned char* bool_fractal = new unsigned char[width*height]; 

    try {

        // Generate the fractal
        boolean_fractal(
            cx,  
            cy, 
            scale, 
            width, 
            height,
            max_iter,
            bool_fractal,
            coeffs
        );

        int surface_area = 0;
        int size = 0;
        int index;
        int rowIndex = 0;

        // Iterate through each pixel in bool_fractal
        // If it doesn't match the pixel below it or to the right then add one to surface area

        for (int row = 0; row<height-1; row++) {            // Exclude bottommost row
            
            for (int col = 0; col<width-1; col++) {         // Exclude rightmost column

                // Find index of current pixel
                index = col + rowIndex;

                // Check for different than right/below
                if (bool_fractal[index] != bool_fractal[index + 1]) {surface_area++;}
                if (bool_fractal[index] != bool_fractal[index+height]) {surface_area++;}

            }

            // Check last pixel in column, only check below
            if (bool_fractal[width - 1 + rowIndex] != bool_fractal[width - 1 + rowIndex + height]) {surface_area++;}

            // Update rowIndex
            rowIndex += width;
        }

        // Check bottom row, only check to the left
        for (int col = 0; col<width-1; col++) {   
                // Find index of current pixel
                index = col + rowIndex;

                // Check for different than right/below
                if (bool_fractal[index] != bool_fractal[index + 1]) {surface_area++;}
        }


        // Find the total number of colored pixels

        rowIndex = 0;

        for (int row = 0; row<height; row++) {     
            for (int col = 0; col<width; col++) { 
                if (bool_fractal[col + rowIndex]) {size++;}
            }
            rowIndex += width;
        }


        // Output results
        *surface_area_out = surface_area;
        *size_out = size;

        delete bool_fractal;
        return 0;
    }
    catch (const std::exception& e) {
        last_error = e.what();
        delete bool_fractal;
        return -1;
    }
}


int surface_area(
    float cx, // X position of center 
    float cy, // Y position of center
    float scale, // Scale of fractal, how wide/long each pixel of the resulting image is in the complex plane
    int width, // Width of resulting image 
    int height, // Height of resulting image
    int max_iter, // Maximum number of iterations
    float* coeffs, // Coefficients to calculate fractal
    int* surface_area_out
) {

    unsigned char* bool_fractal = new unsigned char[width*height]; 

    try {

        // Generate the fractal
        boolean_fractal(
            cx,  
            cy, 
            scale, 
            width, 
            height,
            max_iter,
            bool_fractal,
            coeffs
        );

        int surface_area = 0;
        int index;
        int rowIndex = 0;

        // Iterate through each pixel in bool_fractal
        // If it doesn't match the pixel below it or to the right then add one to surface area

        for (int row = 0; row<height-1; row++) {            // Exclude bottommost row
            
            for (int col = 0; col<width-1; col++) {         // Exclude rightmost column

                // Find index of current pixel
                index = col + rowIndex;

                // Check for different than right/below
                if (bool_fractal[index] != bool_fractal[index + 1]) {surface_area++;}
                if (bool_fractal[index] != bool_fractal[index+height]) {surface_area++;}

            }

            // Check last pixel in column, only check below
            if (bool_fractal[width - 1 + rowIndex] != bool_fractal[width - 1 + rowIndex + height]) {surface_area++;}

            // Update rowIndex
            rowIndex += width;
        }

        // Check bottom row, only check to the left
        for (int col = 0; col<width-1; col++) {   
                // Find index of current pixel
                index = col + rowIndex;

                // Check for different than right/below
                if (bool_fractal[index] != bool_fractal[index + 1]) {surface_area++;}
        }

        // Output results
        *surface_area_out = surface_area;

        delete bool_fractal;
        return 0;
    }
    catch (const std::exception& e) {
        last_error = e.what();
        delete bool_fractal;
        return -1;
    }
}




int standard_julia(
    float cx, // X position of center 
    float cy, // Y position of center
    float scale, // Scale of fractal, how wide/long each pixel of the resulting image is in the complex plane
    int width, // Width of resulting image 
    int height, // Height of resulting image
    int max_iter, // Maximum number of iterations
    int bailout_radius,
    int color_density,
    int* color_palette, // 4x3 array of colors to use for palette, the four rows each are the RGB for a color used
    unsigned char* output,
    float cr,
    float ci,
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

        cl_kernel kernel = ctx.standard_julia_kernel;

        CL_CHECK(clSetKernelArg(kernel, 0, sizeof(float), &cx));
        CL_CHECK(clSetKernelArg(kernel, 1, sizeof(float), &cy));
        CL_CHECK(clSetKernelArg(kernel, 2, sizeof(float), &scale));
        CL_CHECK(clSetKernelArg(kernel, 3, sizeof(int), &width));
        CL_CHECK(clSetKernelArg(kernel, 4, sizeof(int), &height));
        CL_CHECK(clSetKernelArg(kernel, 5, sizeof(int), &max_iter));
        CL_CHECK(clSetKernelArg(kernel, 6, sizeof(int), &bailout_radius));
        CL_CHECK(clSetKernelArg(kernel, 7, sizeof(int), &color_density));
        CL_CHECK(clSetKernelArg(kernel, 8, sizeof(cl_mem), &color_palette_buf));
        CL_CHECK(clSetKernelArg(kernel, 9, sizeof(cl_mem), &out_buf));
        CL_CHECK(clSetKernelArg(kernel, 10, sizeof(int), &cr));
        CL_CHECK(clSetKernelArg(kernel, 11, sizeof(int), &ci));
        CL_CHECK(clSetKernelArg(kernel, 12, sizeof(cl_mem), &coeffs_buf));

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

