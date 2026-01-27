#pragma once

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

DLL_EXPORT const char* get_last_error(void);

DLL_EXPORT int standard_fractal(
    float center_x,
    float center_y,
    float scale,
    int width,
    int height,
    int max_iter,
    int bailout_radius,
    int color_density,
    int* color_palette,
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


#ifdef __cplusplus
}
#endif