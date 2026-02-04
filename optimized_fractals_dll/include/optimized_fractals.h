#pragma once
#include <cstdint>

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

DLL_EXPORT const char* get_last_error(void);

// struct to pass coloring info into standard_fractal
typedef struct {
    int32_t coloring_option;
    int32_t palette[12];
    int32_t density;
} coloring_palette;

DLL_EXPORT int standard_fractal(
    float center_x,
    float center_y,
    float scale,
    int width,
    int height,
    int max_iter,
    int bailout_radius,
    coloring_palette* coloring,
    unsigned char* output,
    float* coeffs
);

DLL_EXPORT int slow_fractal(
    float center_x,
    float center_y,
    float scale,
    int width,
    int height,
    int max_iter,
    unsigned char* output, 
    float* coeffs
);

int boolean_fractal(
    float center_x,
    float center_y,
    float scale,
    int width,
    int height,
    int max_iter,
    unsigned char* output,
    float* coeffs
);

DLL_EXPORT int coolness_raw(
    float cx,
    float cy,
    float scale, 
    int width,
    int height,
    int max_iter,
    float* coeffs,
    int* surface_area_out,
    int* size_out
);


DLL_EXPORT int surface_area(
    float cx,  
    float cy, 
    float scale, 
    int width, 
    int height,
    int max_iter, 
    float* coeffs,
    int* surface_area_out
);

DLL_EXPORT int standard_julia(
    float cx, // X position of center 
    float cy, // Y position of center
    float scale, // Scale of fractal, how wide/long each pixel of the resulting image is in the complex plane
    int width, // Width of resulting image 
    int height, // Height of resulting image
    int max_iter, // Maximum number of iterations
    int bailout_radius,
    int color_density,
    int* color_palette, // 3x3 array of colors to use for palette, the four rows each are the RGB for a color used
    unsigned char* output,
    float cr,
    float ci,
    float* coeffs
);

#ifdef __cplusplus
}
#endif