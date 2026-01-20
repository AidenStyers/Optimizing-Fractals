#pragma once

// include each file in kernels/
#include "kernels/slow_fractal.h"
#include "kernels/standard_fractal.h"

// Add each .h file in kernels/
static const char* all_kernel_sources[] = {
    standard_fractal_source,
    slow_fractal_source
};

static const size_t num_kernel_sources =
    sizeof(all_kernel_sources) / sizeof(all_kernel_sources[0]);